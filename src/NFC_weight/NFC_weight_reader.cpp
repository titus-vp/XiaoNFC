#include "NFC_weight_reader.h"

NFC_weight_reader::NFC_weight_reader() {
    _pn532hsu = new PN532_HSU(Serial1);
    _nfc = new NfcAdapter(*_pn532hsu);
}

bool NFC_weight_reader::begin() {
    // modified _nfc->begin to return bool instead of halting
    // pass false for non-verbose output
    if (!_nfc->begin(true)) {
        return false;
    }
    return true;
}

void NFC_weight_reader::update() {
    if (millis() - _last >= _scan_interval) {
        _last = millis();
        scan();
    }
}

bool NFC_weight_reader::scan(uint16_t timeout) {
    _has_tag = _nfc->tagPresent(timeout);
    return _has_tag;
}

void NFC_weight_reader::read() {
    if (!_has_tag) {
        return;
    }
    NfcTag tag = _nfc->read();

    _uid = tag.getUidString();

    if (!tag.hasNdefMessage()) {
        _has_data = false;
        _has_key = false;
        return;
    }
    NdefMessage message = tag.getNdefMessage();
    _convert_data(message);
}


void NFC_weight_reader::set_scan_interval(int scan_interval) {
    _scan_interval = scan_interval;
}

bool NFC_weight_reader::check_available() const {
    return _has_tag;
}

bool NFC_weight_reader::check_data() const {
    if (!_has_tag) {
        return false;
    }
    return _has_data;
}

bool NFC_weight_reader::check_key() const {
    if (!_has_key) return false;
    return _key == _key_control;
}

String NFC_weight_reader::get_uid() {
    if (!_has_tag) {
        return "";
    }
    return _uid;
}

unsigned int NFC_weight_reader::get_key() const {
    if (!_has_key) {
        return 0;
    }
    return _key;
}

unsigned int NFC_weight_reader::get_weight_id() const {
    if (!_has_data) {
        return 0;
    }
    return _weight_id;
}

unsigned int NFC_weight_reader::get_weight() const {
    if (!_has_data) {
        return 0;
    }
    return _weight;
}

void NFC_weight_reader::_convert_data(NdefMessage &message) {
    unsigned int count = message.getRecordCount();
    if (count < 3) {
        _has_data = false;
        _has_key = false;
        return;
    }
    NdefRecord record = message.getRecord(0);
    _key = _convert_to_int(record);
    _has_key = true;
    if (!check_key()) {
        _has_data = false;
        return;
    }

    record = message.getRecord(1);
    _weight_id = _convert_to_int(record);

    record = message.getRecord(2);
    _weight = _convert_to_int(record);

    _has_data = true;
}

unsigned int NFC_weight_reader::_convert_to_int(NdefRecord &record) {
    // Get length in bytes of payload
    int length = record.getPayloadLength();

    if (length == 0) {
        return 0;
    }

    // allocate memory
    byte * data = (byte*)malloc(length);

    // memcopy of payload into data
    record.getPayload(data);

    // cast data byte array into array of unsigned int AND then get the first element
    unsigned int value = *(unsigned int *)data;

    // free allocated memory
    free(data);

    return value;
}






