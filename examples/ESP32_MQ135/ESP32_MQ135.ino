// sudo chmod a+rw /dev/ttyUSB0
//sudo apt install python-is-python3
//sudo apt install python3-pip
//pip3 install pyserial

#include<MQUnifiedsensor.h>


//This is the server code, the client it is called scanner.
//Profile ->>Service (UUID)->Characteristic(UUID)
//Characteristic --> Property, Value, descriptor (UUID)
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


#define SERVICE_UUID "43dc60ce-42a1-11ee-be56-0242ac120002"

#define CARBON_MONOXIDE_CHAR_UUID "4e771fe2-42a1-11ee-be56-0242ac120000"
#define CARBON_DIOXIDE_CHAR_UUID  "4e771fe2-42a1-11ee-be56-0242ac120001"
#define ALCOHOL_CHAR_UUID         "4e771fe2-42a1-11ee-be56-0242ac120002"
#define ACETONE_CHAR_UUID         "4e771fe2-42a1-11ee-be56-0242ac120003"
#define TOLUEN_CHAR_UUID          "4e771fe2-42a1-11ee-be56-0242ac120004"
#define AMMONIUM_CHAR_UUID        "4e771fe2-42a1-11ee-be56-0242ac120005"

#define CARBON_MONOXIDE_DESC_UUID 0x2901
#define CARBON_DIOXIDE_DESC_UUID  0x2902
#define ALCOHOL_DESC_UUID         0x2903
#define ACETONE_DESC_UUID         0x2904
#define TOLUEN_DESC_UUID          0x2905
#define AMMONIUM_DESC_UUID        0x2906

#define DEVICE_NAME "Smoke Detector"

#define PIN_MQ135 34 //input only

MQUnifiedsensor mq135_sensor(PIN_MQ135, MAX_VALUE_CONVERSOR_ESP_32);

BLEServer* pServer = NULL;

BLECharacteristic* carbonMonoxideChar = NULL;
BLECharacteristic* carbonDioxideChar = NULL;
BLECharacteristic* alcoholChar = NULL;
BLECharacteristic* acetoneChar = NULL;
BLECharacteristic* toluenChar = NULL;
BLECharacteristic* amoniumChar = NULL;

BLEDescriptor *pDescr;
BLE2902 *pBLE2902;

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

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  carbonMonoxideChar = pService->createCharacteristic(
          CARBON_MONOXIDE_CHAR_UUID,
          BLECharacteristic::PROPERTY_NOTIFY |
          BLECharacteristic::PROPERTY_READ |
          //BLECharacteristic::PROPERTY_WRITE |
          BLECharacteristic::PROPERTY_INDICATE
  );
  pDescr = new BLEDescriptor((uint16_t)CARBON_MONOXIDE_DESC_UUID);
  pDescr->setValue("Carbon Monoxide PPM");
  carbonMonoxideChar->addDescriptor(pDescr);
  pBLE2902 = new BLE2902();
  pBLE2902->setNotifications(true);
  carbonMonoxideChar->addDescriptor(pBLE2902);
  
  carbonDioxideChar = pService->createCharacteristic(
          CARBON_DIOXIDE_CHAR_UUID,
          BLECharacteristic::PROPERTY_NOTIFY |
          BLECharacteristic::PROPERTY_READ |
          //BLECharacteristic::PROPERTY_WRITE |
          BLECharacteristic::PROPERTY_INDICATE
  );
  pDescr = new BLEDescriptor((uint16_t)CARBON_DIOXIDE_DESC_UUID);
  pDescr->setValue("Carbon Dioxide PPM");
  carbonDioxideChar->addDescriptor(pDescr);
  pBLE2902 = new BLE2902();
  pBLE2902->setNotifications(true);
  carbonDioxideChar->addDescriptor(pBLE2902);

  alcoholChar = pService->createCharacteristic(
        ALCOHOL_CHAR_UUID,
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_READ |
        //BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_INDICATE
  );
  pDescr = new BLEDescriptor((uint16_t)ALCOHOL_DESC_UUID);
  pDescr->setValue("Alcohol PPM");
  alcoholChar->addDescriptor(pDescr);
  pBLE2902 = new BLE2902();
  pBLE2902->setNotifications(true);
  alcoholChar->addDescriptor(pBLE2902);

  acetoneChar = pService->createCharacteristic(
          ACETONE_CHAR_UUID,
          BLECharacteristic::PROPERTY_NOTIFY |
          BLECharacteristic::PROPERTY_READ |
          //BLECharacteristic::PROPERTY_WRITE |
          BLECharacteristic::PROPERTY_INDICATE
  );
  pDescr = new BLEDescriptor((uint16_t)ACETONE_DESC_UUID);
  pDescr->setValue("Acetone PPM");
  acetoneChar->addDescriptor(pDescr);
  pBLE2902 = new BLE2902();
  pBLE2902->setNotifications(true);
  acetoneChar->addDescriptor(pBLE2902);

  toluenChar = pService->createCharacteristic(
            TOLUEN_CHAR_UUID,
            BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_READ |
            //BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_INDICATE
  );
  pDescr = new BLEDescriptor((uint16_t)TOLUEN_DESC_UUID);
  pDescr->setValue("Toluene PPM");
  toluenChar->addDescriptor(pDescr);
  pBLE2902 = new BLE2902();
  pBLE2902->setNotifications(true);
  toluenChar->addDescriptor(pBLE2902);

  amoniumChar = pService->createCharacteristic(
                      AMMONIUM_CHAR_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_READ |
                      //BLECharacteristic::PROPERTY_WRITE |
                      BLECharacteristic::PROPERTY_INDICATE
  );
  pDescr = new BLEDescriptor((uint16_t)AMMONIUM_DESC_UUID);
  pDescr->setValue("Amonium PPM");
  amoniumChar->addDescriptor(pDescr);
  pBLE2902 = new BLE2902();
  pBLE2902->setNotifications(true);
  amoniumChar->addDescriptor(pBLE2902);

  // this text can be a sensor reading, or the state of a lamp, for example.
  //pCharacteristic->setValue("How can I help you?");
  
  // Start the service
  pService->start();


  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  
  pAdvertising->setScanResponse(false);
  //pAdvertising->setScanResponse(true);

  /**
  Workaround for improving Bluetooth connection compatibility with iPhones.
   iPhones can be quite strict when it comes to Bluetooth connection parameters,
    and sometimes adjustments need to be made to ensure stable connections between an iPhone and a Bluetooth device.
  
  The setMinPreferred function is typically used to set the minimum connection interval preferred by the advertising device.
  The connection interval is the time between two consecutive data exchange events between the Bluetooth devices.
  It plays a crucial role in determining the responsiveness and power consumption of the connection.

  In the Bluetooth specification, the connection interval is specified in units of 1.25 ms,
   and its range is between 7.5 ms and 4,000 ms. iPhones are known to be sensitive to the connection interval,
    and by adjusting this interval to certain values, you can improve the likelihood of a stable connection.

  These values (0x06 and 0x12) are hexadecimal representations of the desired minimum connection interval.
   In decimal, these values are 6 and 18, which correspond to connection intervals of 7.5 ms and 18.75 ms, respectively.
  */
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);

  BLEDevice::startAdvertising();

 
  Serial.println("Waiting a client connection to notify...");
  mq135_sensor.calibrate();
  
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

    // notify changed value
    if (deviceConnected) {
      

        carbonMonoxideChar->setValue(carbonMonoxide);
        carbonMonoxideChar->notify();

        carbonDioxideChar->setValue(carbonDioxide);
        carbonDioxideChar->notify();

        alcoholChar->setValue(alcohol);
        alcoholChar->notify();

        acetoneChar->setValue(acetone);
        acetoneChar->notify();

        toluenChar->setValue(toluen);
        toluenChar->notify();

        amoniumChar->setValue(ammonium);
        amoniumChar->notify();
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