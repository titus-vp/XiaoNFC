#include "NFC_dev.h"

void NFC_dev::waiter(uint16_t timeout) {
    print("Waiting for Tag");
    int i = 0;
    while (!_nfc->tagPresent(timeout)) {
        i++;
        if (i >= _wait * 10) {
            i = 0;
            print(".");
        }
        delay(_wait);
    }
    print("Card found!");
}

void NFC_dev::format() {
    waiter();
    bool success = _nfc->format();
    if (success) {
        print("Success, tag formatted as NDEF.");
    } else {
        print("Format failed.");
    }
}

void NFC_dev::clean() {
    waiter();
    bool success = _nfc->clean();
    if (success) {
        print("Success, tag cleaned.");
    } else {
        print("Cleaning failed.");
    }
}

void NFC_dev::write(unsigned int key, unsigned int weight_id, unsigned int weight) {
    waiter();

    NdefMessage message;

    add_to_message(key, message);
    add_to_message(weight_id, message);
    add_to_message(weight, message);

    _nfc->write(message);

    print("Parameters written!");
}

void NFC_dev::add_to_message(unsigned int value, NdefMessage &message) {
    NdefRecord record;
    record.setPayload((byte*) &value, sizeof(value));
    message.addRecord(record);
}

void NFC_dev::debug(Stream &stream) {
    _debug = &stream;
}

void NFC_dev::print(const String& text) {
    if (!_debug) return;

    _debug->println(text);
}

void NFC_dev::dump() {
    waiter();
    NfcTag tag = _nfc->read();
    tag.print();
}
