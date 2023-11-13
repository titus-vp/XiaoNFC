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
const char* deviceID = "eWeight Coach 00";


ImuData imuData = {};
// buf for encoding the Imu Data into a serial stream with protobuf
uint8_t buf[ImuData_size] = {0};

//Flag for enabling/disabling the Imu data acquisition and characteristic update
bool enableImu = false; 


// set of characteristics - ! Has to be global in the ino for some reason to work...
BLECharacteristic IMU9DofChar("ad0e768f-d4ae-4aa5-97bb-98300a987864", BLERead | BLENotify , sizeof(buf), true); // remote clients will be able to get notifications if this characteristic changes
BLECharacteristic WeightChar("ad0e768f-d4ae-4aa5-97bb-98300a987865", BLERead, sizeof(uint16_t), true);
BLECharacteristic ButtonChar("ad0e768f-d4ae-4aa5-97bb-98300a987866", BLERead | BLENotify ,sizeof(uint16_t), true);

void setup()
{
  Serial.begin(115200);
  BLEinit(deviceID, IMU9DofChar, sizeof(buf), WeightChar, sizeof(uint16_t), ButtonChar, sizeof(uint16_t));
  NFCinit();
  ICMinit();
  buttonInit();
}

void loop()
{
  while(bleIsConntected())
  { 
    pb_ostream_t stream;
    //waitForNFCTag();
    if(tagFound()){
      readWeightTag(totalWeightInGram, weightKey, weightID);
    }
    if(buttonIsPressed()) {
      enableImu = true;
      Serial.println("IMU turned on");
    }
    while(enableImu) { 
      ICMupdate(&imuData);

        // Encode the data using protobuf library
      stream = pb_ostream_from_buffer(buf, sizeof(buf));
      bool encode_status = pb_encode(&stream, ImuData_fields, &imuData);
      if (!encode_status)
      {
        Serial.println("Failed to encode");
        break;
      }

      if(!IMU9DofChar.writeValue(buf, sizeof(buf))){
        Serial.println("Failed to update Characteristic!");
      }
      if(buttonIsPressed()) {
        enableImu = false;
        Serial.println("IMU turned off");
      }
    }
  }
  
  Serial.println("Waiting for connection...");
  while (!bleIsConntected())
  {
    bleWaitForConnection();
    delay(500);  // Warte 500ms, bevor du erneut prüfst.
    }
}




  
