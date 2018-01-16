/*
  SEMeter.h - Library for monitoring electrical energy consumption.
  Created by Andre Souza, October 15, 2016.
  Released into the public domain.
*/
#ifndef Sicee_h
#define Sicee_h

#include "Arduino.h"

class Sicee
{
  public:
    Sicee(int pinVoltage, int pinCurrent);
    float readVoltage();
    float readCurrent(bool _debugSamples);
    void print(float v, float i, float p);
  private:
    int _pinVoltage;
    int _pinCurrent;
};

#endif