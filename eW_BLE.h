#ifndef EW_BLE_H
#define EW_BLE_H


#include <ArduinoBLE.h>

extern BLEDevice central;
// extern BLEService eWeightService;
// extern BLECharacteristic IMU9DofPackage;
// extern BLECharacteristic TotalWeightInGram;
// extern BLECharacteristic ButtonPress;


void BLEinit(const char* deviceID,BLECharacteristic charIMU, int bufferSize,BLECharacteristic charWeight, int valueSizeWeight,BLECharacteristic charButton, int valueSizeButton);

bool bleWaitForConnection();

bool bleIsConntected();

void updateArrCharacteristic(BLECharacteristic characteristic, const uint8_t arr[], int lengthOfArray);

void updateUint16_tCharacteristic(BLECharacteristic characteristic, const uint16_t value);

#endif