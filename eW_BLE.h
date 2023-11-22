#ifndef EW_BLE_H
#define EW_BLE_H


#include <ArduinoBLE.h>
// #include <AdvertisingParameters.h>
// using namespace ble;
// #include <ble/gap/Types.h> 

#define BATTERYPIN 2

extern BLEDevice central;
// extern AdvertisingParameters advParams;
// extern BLEService eWeightService;
// extern BLECharacteristic IMU9DofPackage;
// extern BLECharacteristic TotalWeightInGram;
// extern BLECharacteristic ButtonPress;


void BLEinit(const char* deviceID, BLECharacteristic charIMU, int bufferSize, BLEIntCharacteristic charWeight, int valueSizeWeight, BLECharacteristic charButton, int valueSizeButton);

bool bleWaitForConnection();

bool bleIsConnected();

void updateArrCharacteristic(BLECharacteristic characteristic, const uint8_t arr[], int lengthOfArray);

void updateUint16_tCharacteristic(BLECharacteristic characteristic, const uint16_t value);

//void batLevelInit();

//uint8_t updateBatteryLevel();


#endif