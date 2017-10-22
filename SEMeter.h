/*
  SEMeter.h - Library for monitoring electrical energy consumption.
  Created by Andre Souza, October 15, 2016.
  Released into the public domain.
*/
#ifndef SEMeter_h
#define SEMeter_h

#include "Arduino.h"

class SEMeter
{
  public:
    SEMeter(int pinVoltage, int pinCurrent, int pinVcc);
    float readVoltage();
    float readCurrent(bool _debugSamples);
    float calculatePower(float Vrms, float Irms);
    void serialDebug(float v, float i, float p);
    // float readVoltageTransformer(bool _debugResult, bool _debugSamples);
    // float readCurrentPeak(bool _debugResult, bool _debugSamples);
  private:
    int _pinVoltage;
    int _pinCurrent;
    int _pinVcc;
};

#endif