/*FW for the second Version of the eWeight Wristband including an ICM20948 IMU, PN532 NFC Chip and Xiao Ble nrf52 */
/* Function: Waits for ble Central to connect to -> Waits for an NFC Tag to be read the Weight Value of it and write it into a BLE characteristic 
-> Waits for a button Press, to then acquire, encode IMU Data and update an IMU BLE Characteristic -> a second Buttonpress disables the Data-Aquisition -> wait for NFCTag... */


#include "eW_BLE.h"
#include "eW_ICM.h"
#include "eW_NFC.h"
#include "eW_Feedback.h"
#include "eW_Button.h"

// can be turned on to receive some debug messages but only implemented for eW_ICM.h
#define DEBUG 0


// The name that is used for local and device name of BLE - important for connection with app at this point
const char* deviceID = "eWeight Coach 06";

// Define the desired sample rate for the IMU in Hz
const float desiredSampleRate = 100.0; // Adjust this value as needed

ImuData imuData = {};
// buf for encoding the Imu Data into a serial stream with protobuf
uint8_t buf[ImuData_size] = {0};

//Flag for enabling/disabling the Imu data acquisition and characteristic update
uint8_t enableImu = 0; 
// Flag for error in reading tag
uint8_t tagErrorFlag = 0;

// set of characteristics - ! Has to be global in the ino for some reason to work...
BLECharacteristic IMU9DofChar("ad0e768f-d4ae-4aa5-97bb-98300a987864", BLERead | BLENotify , sizeof(buf), true); // remote clients will be able to get notifications if this characteristic changes
BLEIntCharacteristic WeightChar("ad0e768f-d4ae-4aa5-97bb-98300a987865", BLERead | BLENotify);
BLECharacteristic ButtonChar("ad0e768f-d4ae-4aa5-97bb-98300a987866", BLERead | BLENotify ,sizeof(enableImu), true);

void setup()
{
  Serial.begin(115200);
  BLEinit(deviceID, IMU9DofChar, sizeof(buf), WeightChar, sizeof(uint16_t), ButtonChar, sizeof(uint16_t));
  NFCinit();
  ICMinit();
  buttonInit();
  feedbackInit();
}

void loop() {
  Serial.println(enableImu);
  
  if(totalWeightInGram == 0 && enableImu == 0){
    bLedOn();
    gLedOff();
  }
  
  if(bleIsConnected())
  { 
    
    tagErrorFlag = 0;
    if(totalWeightInGram == 0 && enableImu == 0){
      delay(10);
      if (tagFound(80)) // search for a weight
      {
        if(readWeightTag(totalWeightInGram, weightKey, weightID, tagErrorFlag)){
          //Serial.print("tagErrorFlag = "); Serial.println(tagErrorFlag);

          if(!tagErrorFlag && totalWeightInGram != 0){
            if(WeightChar.writeValue(totalWeightInGram)) // encode weight in g
            {
              delay(50);
              gLedOn();
              vibrationOn();
              delay(100);
              vibrationOff();
            }
          }
        }
        else{
          // if failed to readTag, reset to re-read
          totalWeightInGram = 0;
        }
      }
    }
    // Trigger IMUenable flag upon buttonPress to start IMU Data acquisition
    if (buttonIsPressed() && totalWeightInGram != 0 && enableImu == 0)
    {
      enableImu = 1;
      if(ButtonChar.writeValue(enableImu))
      {
        Serial.println("IMU turned on");
      }
    }


    // Start IMU Data acquisition
    if(enableImu) {
      gLedOn();
      bLedOff();
      if(shouldUpdate(desiredSampleRate)) {
        // get SensorData
        if(ICMupdate(&imuData))
        {
          Serial.println("I am so Updating Eldask");
          // Encode the data using protobuf library
          pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
          bool encode_status = pb_encode(&stream, ImuData_fields, &imuData);
          if (!encode_status) {
            Serial.println("Failed to encode");
          }

          // Update IMU Characteristic
          if (!IMU9DofChar.writeValue(buf, sizeof(buf))) {
            Serial.println("Failed to update Characteristic!");
          } else {
          Serial.println("IMU Char written!");
          calculateSampleRate();
          }
        }
      }
      Serial.println("Checking Button");
      // if buttonIsPressed() stop the data acquisition
      if (buttonIsPressed() && enableImu ) {
        enableImu = 0;
        if(ButtonChar.writeValue(enableImu))
        {
          Serial.println("IMU turned off");
        }
        totalWeightInGram = 0;
      } 
    }
  }
  // Initialize BLE connection if not existing
  if(!bleIsConnected()) {
    bLedOn();
    gLedOff();
    // reset parameter for logic upon dc
    totalWeightInGram = 0;
    enableImu = 0;
    
    Serial.println("Bluetooth® device active, waiting for connections...");
    
    while(!bleIsConnected()){
      bleWaitForConnection();
      delay(500);  // Warte 500ms, bevor du erneut prüfst.
    }
  }
}




  
