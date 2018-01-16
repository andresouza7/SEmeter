#include <siceearduino.h>

#define _voltageAnalogPin 1
#define _currentAnalogPin 2
Sicee meter(_voltageAnalogPin,_currentAnalogPin);

void setup () {
}

void loop (){
    float vrms = meter.readVoltage();
    float irms = meter.readCurrent(false);
    float power = vrms*irms;
    meter.print(vrms,irms,power);
    delay(1000);
}