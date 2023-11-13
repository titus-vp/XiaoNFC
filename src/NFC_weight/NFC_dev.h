#ifndef XIAO_NRF52_NFC_DEV_H
#define XIAO_NRF52_NFC_DEV_H

#include "NFC_weight_reader.h"

class NFC_dev : public NFC_weight_reader {
public:
    void format();
    void clean();
    void dump();

    void write(unsigned int key, unsigned int weight_id, unsigned int weight);

    void waiter();

    void debug(Stream &stream);
private:
    int _wait = 100;

    static void add_to_message(unsigned int value, NdefMessage & message);

    Stream *_debug;
    void print(const String& text);
};


#endif //XIAO_NRF52_NFC_DEV_H
