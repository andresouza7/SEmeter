/*
  SEMeter.cpp - Library for monitoring electric energy consumption.
  Created by Andre Souza, October 15, 2016.
  Released into the public domain.
*/

#include "Arduino.h"
#include "siceearduino.h"

Sicee::Sicee(int pinVoltage, int pinCurrent)
{
  _pinVoltage = pinVoltage;
  _pinCurrent = pinCurrent;
}

float Sicee::readVoltage()
{
  // vrms is found by using the function interpolated through bench experiments
  float vr = float(analogRead(_pinVoltage))*5.00/1024.00;
  // float vrms = -0.2175*pow(vr,4) + 3.15*pow(vr,3) - 16.86*pow(vr,2) + 78.68*vr + 28.5;
  float vrms = -0.2175*pow(vr,4) + 3.15*pow(vr,3) - 16.86*pow(vr,2) + 78.68*vr + 22.5;

  // Values below 170V have shown to have this offset while values above correspond just fine
  // if (vr < 170)
  //   vrms = vrms-0.8;
  // Function interpolated is not very accurate for values below 60V
  if (vrms < 30) vrms=0;

  return vrms;
}

float Sicee::readCurrent(bool _debugSamples)
{
  const float RT = 0.066;
  unsigned long start_time = millis();
  float sum=0;
  float samples=0;
  float m_WeightNew=80.0/100.0;
  float m_Current=0;
  float dc_in = 4.9; // ac/dc power converter output with relay on
  int maxValue = 0;      // store max value
  int minValue = 1024;   // store min value
  while (millis() - start_time < 32) {
    int readValue = analogRead(_pinCurrent);
    if (readValue > maxValue) maxValue = readValue;
    if (readValue < minValue) minValue = readValue;
  }
  int previousValue = analogRead(_pinCurrent);
  while (millis() - start_time < 67) {
    int rawValue = analogRead(_pinCurrent);
    samples++;
    // This part supresses some of the noise for low current values
    if (maxValue < 530 && abs(rawValue - previousValue) < 4) {
    	float current=((rawValue*dc_in/1024.0)-(dc_in/2))/RT;
    	// m_Current = (m_WeightNew * (current) + (1.0 - m_WeightNew) * m_Current);
    	sum=sum+pow(current,2); // Replace "current" with "m_Current" to use exponential filter
    	if (_debugSamples==true) 
			Serial.println(rawValue);
    } else if (maxValue >= 530) {
    	float current=((rawValue*dc_in/1024.0)-(dc_in/2))/RT;
    	// m_Current = (m_WeightNew * (current) + (1.0 - m_WeightNew) * m_Current);
    	sum=sum+pow(current,2); // Replace "current" with "m_Current" to use exponential filter
    	if (_debugSamples==true) 
			Serial.println(rawValue);
    }
	previousValue = rawValue;
  }
  float irms = sqrt(sum/samples);
  if (irms<=0.22) irms=0; // too much noise to get a reliable reading under this value

  return irms;
}

void Sicee::print(float v, float i, float p) {
  Serial.print("Vrms = "); Serial.println(v);
  Serial.print("Irms = "); Serial.println(i);
  Serial.print("Power = "); Serial.println(p);
  Serial.println("--------------------------");
}