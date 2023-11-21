#ifndef EW_NFC_H
#define EW_NFC_H

#include "src/NFC_weight/NFC_dev.h"

extern NFC_dev NFC_DEV;

extern float totalWeightInGram;
extern uint8_t weightKey;
extern uint8_t weightID;

void NFCinit();

void waitForNFCTag(uint16_t timeout = 0);

bool tagFound(uint16_t timeout = 0);
 
bool readWeightTag(float &totalWeightInGram, uint8_t &weightID, uint8_t &weightKey, uint8_t &errorFlag);

#endif