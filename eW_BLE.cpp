#include "eW_BLE.h"

BLEDevice central;
//AdvertisingParameters advParams;


BLEService eWeightService("49cc6a9e-0fa3-493b-b290-e1ac59909dec");
//BLEService batteryService("180F");


//uint8_t oldBatteryPercentage = 0;  // last battery level reading from analog input
//long previousMillis = 0;  // last time the battery level was checked, in ms

void BLEinit(const char* deviceID, BLECharacteristic charIMU, int bufferSize, BLEIntCharacteristic charWeight, int valueSizeWeight,BLECharacteristic charButton, int valueSizeButton)
{ 
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }
  //advertising_power_t tx_power = 4; // dBm // maybe not set, but doesnt affect rssi 
  //advParams.setTxPower(tx_power);
  BLE.setDeviceName(deviceID);
  BLE.setLocalName(deviceID);


   //byte data[5] = { 0x0e, 0x0e, 0x0e}; //doesnt work from arduino side right now
 // BLE.setManufacturerData(data, 3);
  BLE.setAdvertisedService(eWeightService); // add the service UUID
  eWeightService.addCharacteristic(charIMU); // 
  eWeightService.addCharacteristic(charWeight);
  eWeightService.addCharacteristic(charButton);

  //BLE.setAdvertisedService(batteryService);
  //batteryService.addCharacteristic(batteryPercentageChar);

  BLE.addService(eWeightService); // Add the service
  //BLE.addService(batteryService);

  uint8_t zeroArray[bufferSize] = {0};
  charIMU.writeValue(zeroArray, bufferSize); // set initial value for this characteristics
  delay(10);

  uint16_t initial_weight_g = 0;
  charWeight.writeValue(initial_weight_g);
  delay(10);
  
  uint16_t initialButtonVal = 0;
  charButton.writeValue(initialButtonVal);
  delay(10);

  //batteryPercentageChar.writeValue(oldBatteryPercentage);
  //delay(10);

  // start advertising
  if(BLE.advertise()){

     Serial.println("Bluetooth® device active, waiting for connections...");
  }
}

bool bleWaitForConnection()
{
  // wait for a Bluetooth® Low Energy central
  central = BLE.central();


  // if a central is connected to the peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's BT address:
    Serial.println(central.address());
    // turn on the LED to indicate the connection:
    //digitalWrite(LED_BUILTIN, HIGH);
    
    if(central.hasService("49cc6a9e-0fa3-493b-b290-e1ac59909dec")){
      if(central.characteristicCount() == 3){
        return true;
      }
    }
  } else {
    return false;
  }
}

bool bleIsConnected()
{
  if (central.connected()){

    return true;
  }
  else{
    return false;
  }
}

void updateArrCharacteristic(BLECharacteristic characteristic, const uint8_t arr[], int lengthOfArray)
{
  if(!characteristic.writeValue(arr, lengthOfArray)) {
    Serial.println("Failed to update Arr Characteristic");
  }
}
void updateUint16_tCharacteristic(BLECharacteristic characteristic, const uint16_t value)
{
  if(!characteristic.writeValue(value)) {
    Serial.println("Failed to update Arr Characteristic");
  }
}

//void batLevelInit(){
  //analogAcquisitionTime(AT_15_US);
  //analogReference(AR_INTERNAL2V4);
//}


// // shows realistiv values - anyway has to be further evaluated before usage
// uint8_t updateBatteryLevel() {
//   const float vRef = 2.4; // V
//   int adcVal = analogRead(BATTERYPIN);
//   float voltage = 2*(adcVal * vRef) / 1023;
//   Serial.print("Battery Voltage = ");Serial.print(voltage); Serial.println("V");
  
//   uint8_t batteryPercentage = map(voltage*10, 32 , 42, 0, 100);
//   return batteryPercentage;
// }


