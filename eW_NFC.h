#ifndef EW_NFC_H
#define EW_NFC_H

#include "src/NFC_weight/NFC_dev.h"

extern NFC_dev NFC_DEV;

extern uint16_t totalWeightInGram;
extern uint8_t weightKey;
extern uint8_t weightID;

void NFCinit();

void waitForNFCTag();

bool tagFound();
 
bool readWeightTag(uint16_t &totalWeightInGram, uint8_t &weightID, uint8_t &weightKey);


#endif