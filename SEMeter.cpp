/*
  SEMeter.cpp - Library for monitoring electric energy consumption.
  Created by Andre Souza, October 15, 2016.
  Released into the public domain.
*/

#include "Arduino.h"
#include "SEMeter.h"

SEMeter::SEMeter(int pinVoltage, int pinCurrent, int pinVcc)
{
  // pinMode(pinVoltage, OUTPUT);
  // pinMode(pinCurrent, OUTPUT);
  _pinVoltage = pinVoltage;
  _pinCurrent = pinCurrent;
  _pinVcc = pinVcc;
}

float SEMeter::readVoltage()
{
  // vrms is found by using the function interpolated through bench experiments
  float vcc = 5.00*float(analogRead(_pinVcc))/1024.00;
  float vr = float(analogRead(_pinVoltage))*5.00/1024.00;
  // float vrms = -0.2175*pow(vr,4) + 3.15*pow(vr,3) - 16.86*pow(vr,2) + 78.68*vr + 28.5;
  float vrms = -0.2175*pow(vr,4) + 3.15*pow(vr,3) - 16.86*pow(vr,2) + 78.68*vr + 22.5;

  // Values below 170V have shown to have this offset while values above correspond just fine
  // if (vr < 170)
  //   vrms = vrms-0.8;
  // Function interpolated is not very accurate for values below 60V
  if (vrms < 60) vrms=0;

  return vrms;
}

float SEMeter::readCurrent(bool _debugSamples)
{
  const float RT = 0.066;
  unsigned long start_time = millis();
  // Finds the current axis midpoint to prevent error due to fluctuation
  float sum=0;
  float samples=0;
  float m_WeightNew=80.0/100.0;
  float m_Current=0;
  int maxValue = 0;      // store max value
  int minValue = 1024;   // store min value
  while (millis() - start_time < 32) {
    int readValue = analogRead(_pinCurrent);
    if (readValue > maxValue) maxValue = readValue;
    if (readValue < minValue) minValue = readValue;
  }
  while (millis() - start_time < 67) {
    int rawValue = analogRead(_pinCurrent);
    samples++;
    float current=((rawValue*5.0/1024.0)-(float(maxValue+minValue)/2.0)*5.0/1024.0)/0.063;
    m_Current = (m_WeightNew * (current) + (1.0 - m_WeightNew) * m_Current);
    sum=sum+pow(m_Current,2); // Replace "current" with "m_Current" to use exponential filter
    if (_debugSamples==true) Serial.println(rawValue);
  }
  float irms = sqrt(sum/samples);
  if (irms<=0.08) irms=0;

  return irms;
}

void SEMeter::serialDebug(float v, float i, float p) {
  Serial.print("Vrms = "); Serial.println(v);
  Serial.print("Irms = "); Serial.println(i);
  Serial.print("Power = "); Serial.println(p);
  Serial.println("--------------------------");
}

float SEMeter::calculatePower(float Vrms, float Irms)
{
  float power = Vrms*Irms;
  return power;
}

// ======================================================================
// THE FUNCTIONS BELOW ARE OBSOLETE AND NO LONGER USED IN THE CODE

// float SEMeter::readVoltageTransformer(bool _debugResult, bool _debugSamples)
// {
//   // -------- Experimental measurements ---------
//   // Transformer primary winding = 127.6 V
//   // Transformer secondary = 7.6 V
//   // Transformer voltage divider = 1.5 V
//   // RTP = 7.77; //transformer windings ratio
//   // RDT = 10.9; //voltage divider ratio (between 10.4 and 11.14)
//   // -------- Experimental measurements ---------

//   const double RTP = 16.7;
//   const double RDT = 10.7;

//   const double calibrationConstant = RTP*RDT;

//   unsigned long start_time = millis();
//   // Finds the current axis midpoint to prevent error due to fluctuation
//   float sum=0;
//   float samples=0;
//   float m_WeightNew=90.0/100.0;
//   float m_Current=0;
//   while (millis() - start_time < 67) {
//     int rawValue = analogRead(_pinVoltage);
//     samples++;
//     float voltage=(rawValue*5.0/1023.0)-2.54;
//     m_Current = (m_WeightNew * (voltage*RTP*RDT) + (1.0 - m_WeightNew) * m_Current);
//     sum=sum+pow(m_Current,2);
//   }
//   float vrms = sqrt(sum/samples);
//   return vrms;
// }

// // This function applys a direct conversion by dividing value measured by sqrt(2), which gives the 
// // Rrms value of the current. However, this only works for a pure sinudoidal waveform.
// float SEMeter::readCurrentPeak(bool _debugResult, bool _debugSamples)
// {
//   float noise=0.09;      // error / minimum measurement interval 66mA + tolerance
//   int maxValue = 0;      // store max value
//   int minValue = 1023;   // store min value
//   bool firstSample = true;
//   unsigned long start_time = millis();
//   const float Vsupply = 4.86; // Voltage at current sensor Vcc pin

//   int lastValue=0;
//   while (millis() - start_time < 67) {  //sample 4 cycles for 80 ms/50Hz, 67ms/60Hz
//     int readValue = analogRead(_pinCurrent);
//     if (_debugSamples==true) Serial.println(readValue);
//     if (firstSample==true) {
//       maxValue=readValue;
//       minValue=readValue;
//       firstSample=false;
//     }
//     // Skips random sample errors
//     if (readValue-lastValue < 25) {
//       if (readValue > maxValue) {maxValue = readValue;}
//     } 
//     if (lastValue-readValue < 25) {
//       if (readValue < minValue) {minValue = readValue;}
//     } 
//     lastValue = readValue;
//   }
//   float max = float(maxValue);
//   float min = float(minValue);
//   float Iraw = (((max - min) * Vsupply / 1023.0) / 2.0) * 0.7071;
//   float Irms = (Iraw * 1000.0) / 66.0; // 100 - sens 20A; 66 - sens 30A; 185 - sens 5A
//  // Irms = 0.05 * (int) ( (Irms) / 0.05); // rounded 50mA, NOT MANDATORY
//   if (Irms<=noise)
//     Irms=0;
//   //debug
//   if (_debugSamples==true) {
//     Serial.print("max value ");
//   Serial.println(maxValue);
//   Serial.print("min value ");
//   Serial.println(minValue);
//   }
//   if (_debugResult==true) {
//   Serial.print("Irms: ");
//     Serial.println(Irms,3);
//   }
//   return Irms;
// }