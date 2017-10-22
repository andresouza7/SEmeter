#include <SEMeter.h>

#define _PinoVcc 0
#define _PinoAnalogicoTensao 1
#define _PinoAnalogicoCorrente 2
SEMeter meter(_PinoVcc,_PinoAnalogicoTensao,_PinoAnalogicoCorrente);

void setup () {
}

void loop (){
    // readCurrent(value) is for Serial debugging of each 
    // current sample
    // readCurrent(value) é para imprimir no monitor serial
    // o valor de CADA amostra de corrente
    float vrms = meter.readVoltage();
    float irms = meter.readCurrent(false);
    float power = vrms*irms;
    meter.serialDebug(vrms,irms,power);
    delay(1000);
}
