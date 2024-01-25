// sudo chmod a+rw /dev/ttyUSB0
//sudo apt install python-is-python3
//sudo apt install python3-pip
//pip3 install pyserial

//ESP32-WROOM-DA MOdule
//100 during upload
#include<MQUnifiedsensor.h>


//This is the server code, the client it is called scanner.
//Profile ->>Service (UUID)->Characteristic(UUID)
//Characteristic --> Property, Value, descriptor (UUID)
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

//SERVICE_UUID
#define FIRST_UUID "43dc60ce-42a1-11ee-be56-0242ac120001"
#define SECOND_UUID  "43dc60ce-42a2-11ee-be56-0242ac120001"
#define THRID_UUID         "43dc60ce-42a3-11ee-be56-0242ac120001"
#define FOUTH_UUID         "43dc60ce-42a4-11ee-be56-0242ac120001"

#define CARBON_MONOXIDE_CHRC_UUID "43dc60ce-42b1-11ee-be56-0242ac120001"
#define CARBON_DIOXIDE_CHRC_UUID  "43dc60ce-42b2-11ee-be56-0242ac120001"
#define ALCOHOL_CHRC_UUID         "43dc60ce-42b3-11ee-be56-0242ac120001"
#define ACETONE_CHRC_UUID         "43dc60ce-42b4-11ee-be56-0242ac120001"
#define TOLUENE_CHRC_UUID          "43dc60ce-42b5-11ee-be56-0242ac120001"
#define AMMONIUM_CHRC_UUID        "43dc60ce-42b6-11ee-be56-0242ac120001"
#define PERCENTAGE_CHRC_UUID      "43dc60ce-42b7-11ee-be56-0242ac120001"

#define CARBON_MONOXIDE_DESC_UUID 0x3901
#define CARBON_DIOXIDE_DESC_UUID  0x3902
#define ALCOHOL_DESC_UUID         0x3903
#define ACETONE_DESC_UUID         0x3904
#define TOLUENE_DESC_UUID          0x3905
#define AMMONIUM_DESC_UUID        0x3906
#define PERCENTAGE_DESC_UUID      0x3907

#define DEVICE_NAME "Air Quality Detector"

#define PIN_MQ135 34 //input only

MQUnifiedsensor mq135_sensor(PIN_MQ135, MAX_VALUE_CONVERSOR_ESP_32);

BLEServer* pServer = NULL;

#define CARBON_MONOXIDE_INDEX 0
#define CARBON_DIOXIDE_INDEX 1
#define ALCOHOL_INDEX 2
#define ACETONE_INDEX 3
#define TOLUENE_INDEX 4
#define AMMONIUM_INDEX 5
#define PERCENTAGE_INDEX 6

//BLECharacteristic* percentageChar = NULL;
BLECharacteristic* arrayCharacteristics [7];
 

BLEService* serviceList [4];
//BLEDescriptor *pDescr;

bool deviceConnected = false;
bool oldDeviceConnected = false;


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};


void createService ( char * SERVICE_UUID, int positionIndex){
 // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  serviceList [positionIndex] = pService;

  // Start the service
  //pService->start();
  return;
}

void startService (int positionIndex){
  BLEService *pService = serviceList [positionIndex];

  // Start the service
  pService->start();
  return;
}

BLECharacteristic* createCharacteristic (int positionIndex, char* CHAR_UUID, uint16_t DESC_UUID, char* value ){
  BLEService *pService = serviceList [positionIndex];
 // Create a BLE Characteristic
  BLECharacteristic* characteristic = pService->createCharacteristic(
          CHAR_UUID,
          BLECharacteristic::PROPERTY_NOTIFY |
          BLECharacteristic::PROPERTY_READ
          //BLECharacteristic::PROPERTY_WRITE |
          //BLECharacteristic::PROPERTY_INDICATE
  );
  BLEDescriptor * pDescr = new BLEDescriptor(DESC_UUID);
  pDescr->setValue(value); // uuid 0000xxxx-0000-1000-8000-00805F9B34FB 
  characteristic->addDescriptor(pDescr); //additional fixed metadata
  BLE2902 * pBLE2902 = new BLE2902(); //CCCD uuid 00002902-0000-1000-8000-00805F9B34FB 
  pBLE2902->setNotifications(true); //this enadle the notifications
  characteristic->addDescriptor(pBLE2902);

  return characteristic;
}

void setup() {
  //blue led
  pinMode(2,OUTPUT);
  mq135_sensor.init(); 
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  // Create the BLE Device
  BLEDevice::init(DEVICE_NAME);

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  createService ( FIRST_UUID, 0);
  // Create a BLE Characteristic
  arrayCharacteristics[CARBON_MONOXIDE_INDEX] = createCharacteristic (0, CARBON_MONOXIDE_CHRC_UUID,
    (uint16_t)CARBON_MONOXIDE_DESC_UUID,"Carbon Monoxide PPM");

  arrayCharacteristics[CARBON_DIOXIDE_INDEX] = createCharacteristic ( 0, CARBON_DIOXIDE_CHRC_UUID,
    (uint16_t)CARBON_DIOXIDE_DESC_UUID,"Carbon Dioxide PPM");

  startService (0);
  createService ( SECOND_UUID, 1);
  arrayCharacteristics[ALCOHOL_INDEX] = createCharacteristic ( 1, ALCOHOL_CHRC_UUID,
    (uint16_t)ALCOHOL_DESC_UUID,"Alcohol PPM");

  arrayCharacteristics[ACETONE_INDEX] = createCharacteristic ( 1, ACETONE_CHRC_UUID,
    (uint16_t)ACETONE_DESC_UUID,"Acetone PPM");
  startService (1);
createService ( THRID_UUID, 2);
    
  arrayCharacteristics[TOLUENE_INDEX] = createCharacteristic ( 2, TOLUENE_CHRC_UUID,
    (uint16_t)TOLUENE_DESC_UUID,"Toluene PPM");

  arrayCharacteristics[AMMONIUM_INDEX] = createCharacteristic ( 2, AMMONIUM_CHRC_UUID,
    (uint16_t)AMMONIUM_DESC_UUID,"Amonium PPM");
  
  startService (2);
  createService ( FOUTH_UUID, 3);
  arrayCharacteristics[6] = createCharacteristic ( 3, PERCENTAGE_CHRC_UUID,
    (uint16_t)PERCENTAGE_DESC_UUID,"Percentage");
  startService (3);

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(FIRST_UUID);
  pAdvertising->addServiceUUID(SECOND_UUID);
  pAdvertising->addServiceUUID(THRID_UUID);
  pAdvertising->addServiceUUID(FOUTH_UUID);
  //it has 6 service max. after this number it add a new char to the last service. 
  pAdvertising->setScanResponse(false);
  //pAdvertising->setScanResponse(true);

  /**
  Workaround for improving Bluetooth connection compatibility with iPhones.

  These values (0x06 and 0x12) are hexadecimal representations of the desired minimum connection interval.
   In decimal, these values are 6 and 18, which correspond to connection intervals of 7.5 ms and 18.75 ms, respectively.
  */
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMaxPreferred(0x12);
  
  mq135_sensor.calibrate();
  Serial.println("Calibrating...");

  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}


void loop() {
    digitalWrite(2, HIGH); // Blue LED on
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

  float percentage = mq135_sensor.getSensedPercentage();

    // notify changed value
    if (deviceConnected) {
        arrayCharacteristics[CARBON_MONOXIDE_INDEX]->setValue(carbonMonoxide);
        arrayCharacteristics[CARBON_MONOXIDE_INDEX]->notify();

        arrayCharacteristics[CARBON_DIOXIDE_INDEX]->setValue(carbonDioxide);
        arrayCharacteristics[CARBON_DIOXIDE_INDEX]->notify();

        arrayCharacteristics[ALCOHOL_INDEX]->setValue(alcohol);
        arrayCharacteristics[ALCOHOL_INDEX]->notify();

        arrayCharacteristics[ACETONE_INDEX]->setValue(acetone);
        arrayCharacteristics[ACETONE_INDEX]->notify();

        arrayCharacteristics[TOLUENE_INDEX]->setValue(toluen);
        arrayCharacteristics[TOLUENE_INDEX]->notify();

        arrayCharacteristics[AMMONIUM_INDEX]->setValue(ammonium);
        arrayCharacteristics[AMMONIUM_INDEX]->notify();

        arrayCharacteristics[6]->setValue(percentage);
        arrayCharacteristics[6]->notify();

        Serial.println("percentage");
        Serial.println(percentage);
        delay(1000);
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting...
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
     digitalWrite(2, LOW); // Blue LED off
}