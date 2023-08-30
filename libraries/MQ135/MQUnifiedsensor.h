#ifndef MQUnifiedsensor_H
  #define MQUnifiedsensor_H

#include <Arduino.h>
#include <stdint.h>

/***********************Software Related Macros************************************/

class MQUnifiedsensor
{
  public:
    //Resolution of 10 bit => 2^N-1 = 1023
    #define MAX_VALUE_CONVERSOR_ARDUINO_UNO 1023.0
    //Resolution of 12 bit => 2^N-1 = 4095
    #define MAX_VALUE_CONVERSOR_ESP_32 4095.0
    MQUnifiedsensor( int pin = 1, float maxValue = MAX_VALUE_CONVERSOR_ARDUINO_UNO);

    //Functions to set values
    void init();
    float getSensedPercentage();

    #define CARBON_MONOXIDE 0
    #define CARBON_DIOXIDE 1
    #define ALCOHOL 2
    #define ACETONE 3
    #define TOLUEN 4
    #define AMMONIUM 5
    void setGas(int gas);

    //user functions
    #define RatioMQ135CleanAir 3.6//RS / R0 = 3.6 ppm
    void calibrate(float ratioInCleanAir = RatioMQ135CleanAir);
    float readSensorInPPM(float correctionFactor = 0.0);

    // functions for testing
    float getResistenceInGas();
    /// Parameters to model temperature and humidity dependence
    #define CORA 0.00035
    #define CORB 0.02718
    #define CORC 1.39538
    #define CORD 0.0018
    float getCorrectionFactor(float temperature, float humidity);
  private:
    /************************Private vars************************************/
    byte _pin = 1;
    float _max_value_convesor;
		
    #define LOAD_RESISTENCE 10.0 //Value in KiloOhms
    //factors depending the type of GAS.
    float _a, _b;
    float  _resistenceOfSensorInCleanAir;

};

#endif //MQUnifiedsensor_H
