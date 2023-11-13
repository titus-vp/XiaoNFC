#include "eW_BLE.h"

BLEDevice central;
BLEService eWeightService("49cc6a9e-0fa3-493b-b290-e1ac59909dec");


void BLEinit(const char* deviceID, BLECharacteristic charIMU, int bufferSize, BLECharacteristic charWeight, int valueSizeWeight,BLECharacteristic charButton, int valueSizeButton)
{ 

  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }
  
  BLE.setDeviceName(deviceID);
  BLE.setLocalName(deviceID);

   //byte data[5] = { 0x0e, 0x0e, 0x0e}; //doesnt work from arduino side right now
 // BLE.setManufacturerData(data, 3);
  BLE.setAdvertisedService(eWeightService); // add the service UUID
  eWeightService.addCharacteristic(charIMU); // 
  eWeightService.addCharacteristic(charWeight);
  eWeightService.addCharacteristic(charButton);
  
  BLE.addService(eWeightService); // Add the service

  uint8_t zeroArray[bufferSize] = {0};
  charIMU.writeValue(zeroArray, bufferSize); // set initial value for this characteristics
  delay(10);

  uint16_t initial_weight_g = 0;
  charWeight.writeValue(initial_weight_g);
  delay(10);
  
  uint16_t initialButtonVal = 0;
  charButton.writeValue(initialButtonVal);
  delay(10);

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
    
    return true;
  }
  else{
    return false;
  }
}

bool bleIsConntected()
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
