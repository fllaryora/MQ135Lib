#include "MQUnifiedsensor.h"

#define retries 2
#define retry_interval 20

MQUnifiedsensor::MQUnifiedsensor(int pin, float maxValue) {
  this->_pin = pin;
  this->_max_value_convesor = maxValue;
}

void MQUnifiedsensor::init(){
  pinMode(_pin, INPUT);
}

//Setting Exponential regression (a, b) values 
//https://www.codrey.com/electronic-circuits/how-to-use-mq-135-gas-sensor/
void MQUnifiedsensor::setGas(int gas) {

  switch(gas) {
     case CARBON_MONOXIDE:
       this->_a = 605.18;
       this->_b = -3.937;
       break;
     case CARBON_DIOXIDE:
       this->_a = 110.47;
       this->_b = -2.862;
       break;
     case ALCOHOL:
       this->_a = 77.255;
       this->_b = -3.18;
       break;
     case ACETONE:
       this->_a = 34.668;
       this->_b = -3.369;
       break;
     case TOLUEN:
       this->_a = 44.947;
       this->_b = -3.445;
       break;
     case AMMONIUM:
       this->_a = 102.2;
       this->_b = -2.473;
       break;
     default://CARBON_DIOXIDE
       this->_a = 110.47;
       this->_b = -2.862;
       break;
   }
}

//https://github.com/miguel5612/MQSensorsLib
//http://davidegironi.blogspot.com/2017/05/mq-gas-sensor-correlation-function.html
//https://www.codrey.com/electronic-circuits/how-to-use-mq-135-gas-sensor/#google_vignette
//Exponential regression mode
float MQUnifiedsensor::readSensorInPPM( float correctionFactor) {
  float ratio = getResistenceInGas() / this->_resistenceOfSensorInCleanAir;
  ratio += correctionFactor;
  if(ratio <= 0)  ratio = 0;
  float ppm = _a * pow(ratio, _b);
  if(ppm < 0)  ppm = 0;
  return ppm;
}

void MQUnifiedsensor::calibrate(float ratioInCleanAir) {
  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for(int i = 1; i <= 10; i ++) {
    float resistenceOfSensorInCleanAir = getResistenceInGas() / ratioInCleanAir;
    if(resistenceOfSensorInCleanAir < 0.0) resistenceOfSensorInCleanAir = 0.0;
    calcR0 += resistenceOfSensorInCleanAir;
    Serial.print(".");
  }
  if(isinf(calcR0)) {
     Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply");
     while(1);
  }
  if(calcR0 == 0){
     Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply");
     while(1);
  }
  // Ro = sensor resistance at 100ppm of NH3 in the clean air
  this->_resistenceOfSensorInCleanAir = calcR0 / 10.0 ;
}

float MQUnifiedsensor::getSensedPercentage() {

  float avg = 0.0;
  for (int i = 0; i < retries; i ++) {
    float adc = analogRead(this->_pin);
    avg += adc;
    delay(retry_interval);
  }

  return (avg/ retries) / this->_max_value_convesor;
}

//Rs means resistance in target gas with different concentration,
float MQUnifiedsensor::getResistenceInGas() {
  //Get value of RS in a gas
  float resistenceCalculated = ( LOAD_RESISTENCE / getSensedPercentage() ) - LOAD_RESISTENCE;
  if(resistenceCalculated < 0)
    resistenceCalculated = 0;
  return resistenceCalculated;
}

/**************************************************************************/
/*!
@brief  Get the correction factor to correct for temperature and humidity
@param[in] temperature  The ambient air temperature
@param[in] humidity  The relative humidity
@return The calculated correction factor
*/
/**************************************************************************/
float MQUnifiedsensor::getCorrectionFactor(float temperature, float humidity) {
  return CORA * temperature * temperature - CORB * temperature + CORC - (humidity-33.)*CORD;
}
