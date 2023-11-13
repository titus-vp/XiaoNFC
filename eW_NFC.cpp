#include "eW_NFC.h"

NFC_dev NFC_DEV;

uint16_t totalWeightInGram = 0;
uint8_t weightKey = 0;
uint8_t weightID = 0;


void NFCinit()
{
  NFC_DEV.begin();
  NFC_DEV.debug(Serial);
  Serial.println("Setup Done!");
}

void waitForNFCTag()
{
  NFC_DEV.waiter();
}

bool tagFound()
{
  if (!NFC_DEV.scan()) {
    Serial.println("No card?");
    return false;
  }
  else{
    return true;
  }
}

bool readWeightTag(uint16_t &totalWeightInGram, uint8_t &weightID, uint8_t &weightKey)
{
  NFC_DEV.read();

  if (!NFC_DEV.check_data()) {
    Serial.println("No data read");
    return false;
  }

  totalWeightInGram = NFC_DEV.get_weight();
  Serial.print("Weight: ");
  Serial.println(totalWeightInGram);

  weightID = NFC_DEV.get_weight_id();
  Serial.print("ID: ");
  Serial.println(weightID);

  weightKey = NFC_DEV.get_key();
  Serial.print("Key: ");
  Serial.println(weightKey);
  return true;
}