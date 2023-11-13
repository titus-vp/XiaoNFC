#ifndef XIAO_NRF_NFC_WEIGHT_READER_H
#define XIAO_NRF_NFC_WEIGHT_READER_H

#include <PN532_HSU.h>
#include <PN532.h>
#include <NfcAdapter.h>

class NFC_weight_reader {
public:
    NFC_weight_reader();

    bool begin();
    void update();

    bool scan();

    void read();

    void set_scan_interval(int scan_interval);

    bool check_available() const;

    bool check_data() const;
    bool check_key() const;

    String get_uid();
    unsigned int get_weight_id() const;
    unsigned int get_weight() const;
    unsigned int get_key() const;

protected:
    PN532_HSU * _pn532hsu;
    NfcAdapter * _nfc;

private:
    bool _has_tag = false;
    bool _has_data = false;
    bool _has_key = false;

    String _uid;

    unsigned int _key_control = 1234;

    unsigned int _key{};
    unsigned int _weight_id{};
    unsigned int _weight{}; // kg


    int _scan_interval = 100;
    unsigned long _last = 0;

    void _convert_data(NdefMessage &message);

    static unsigned int _convert_to_int(NdefRecord &record);

};


#endif //XIAO_NRF_NFC_WEIGHT_READER_H
