/*
 * Atmospheric CO2 Level..............400ppm
 * Average indoor co2.............350-450ppm
 * Maxiumum acceptable co2...........1000ppm
 * Dangerous co2 levels.............>2000ppm
 */
#include<MQUnifiedsensor.h>

#define PIN_MQ135 A0

MQUnifiedsensor mq135_sensor(PIN_MQ135, MAX_VALUE_CONVERSOR_ARDUINO_UNO);

float temperature = 21.0; // Assume current temperature. Recommended to measure with DHT22
float humidity = 25.0; // Assume current humidity. Recommended to measure with DHT22

void setup(){
  Serial.begin(9600);
  mq135_sensor.init(); 
  mq135_sensor.calibrate();
}

void loop(){
  Serial.println("MQ135: ");

  //Carbon monoxide( CO ) 
  mq135_sensor.setGas(CARBON_MONOXIDE);
  float carbonMonoxide = mq135_sensor.readSensorInPPM();

  //Alcohol (C2H6O) 
  mq135_sensor.setGas(ALCOHOL);
  float alcohol = mq135_sensor.readSensorInPPM();


  //Carbon dioxide CO2
  mq135_sensor.setGas(CARBON_DIOXIDE);
  float carbonDioxide = mq135_sensor.readSensorInPPM();


  // Toluen (C6H5CH3) 
  mq135_sensor.setGas(TOLUEN);
  float toluen = mq135_sensor.readSensorInPPM();


  // ammonium ion(NH4)
  mq135_sensor.setGas(AMMONIUM);
  float ammonium = mq135_sensor.readSensorInPPM();
  //Ammonia is(NH3)

  //  Acetone (C3H6O) 
  mq135_sensor.setGas(ACETONE);
  float acetone = mq135_sensor.readSensorInPPM();

  Serial.print("MQ135: ");
  Serial.print("Carbon Monoxide: ");
  Serial.print(carbonMonoxide);
  Serial.println(" ppm");

  Serial.print("Alcohol: ");
  Serial.print(alcohol);
  Serial.println(" ppm");

  Serial.print("Carbon Dioxide: ");
  Serial.print(carbonDioxide + 400);
  Serial.println(" ppm");

  Serial.print("Toluen: ");
  Serial.print(toluen); 
  Serial.println(" ppm");
  
  Serial.print("Ammonium:");
  Serial.print(ammonium);
  Serial.println(" ppm");

  Serial.print("Acetone: ");
  Serial.print(acetone);
  Serial.println(" ppm");

  delay(500); //Sampling frequency                                   //   wait 100ms for next reading
}
