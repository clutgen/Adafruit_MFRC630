#include "Adafruit_MFRC630.h"
#include <iomanip>

void print_buf_hex(const unsigned char* buf, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        printf("%02X ", buf[i]);
    }
    printf("\n");
}

/* Dumps an entire sector (4*16-byte blocks) to the serial monitor. */
void radio_mifare_dump_sector(Adafruit_MFRC630 reader, uint8_t sector_num) {
    uint8_t readbuf[16] = { 0 };
    /* Try to read four blocks inside the sector. */
    for (uint8_t b = 0; b < 4; b++) {
        uint8_t len = 0;
        len = reader.mifareReadBlock(sector_num * 4 + b, readbuf);
        if (len == 0) {
            /* No data returned! */
            std::cout << "What!?! No data returned for block " << static_cast<int>(sector_num * 4 + b) << "!" << std::endl;
            #if MOJIC_TRICK
            std::cout << "(ノ ゜Д゜)ノ ︵ ┻━┻" << std::endl;
            #endif
            return;
        } else {
            /* Display the block contents. */
            std::cout << static_cast<int>(sector_num * 4 + b) << ": ";
            print_buf_hex(readbuf, len);
        }
    }
}

int main() {
    bool rc;

    Adafruit_MFRC630 reader(0x28, -1);

    /* Put the IC in a known-state. */
    reader.softReset();

    /* Configure the radio for ISO14443A-106. */
    reader.configRadio(MFRC630_RADIOCFG_ISO1443A_106);

    /* Request a tag (activates the near field, etc.). */
    uint16_t atqa = reader.iso14443aRequest();

    /* Looks like we found a tag, move on to selection. */
    if (atqa)
    {
        uint8_t uid[10] = { 0 };
        uint8_t uidlen;
        uint8_t sak;

        /* Retrieve the UID and SAK values. */
        uidlen = reader.iso14443aSelect(uid, &sak);
        // std::cout << "Found a tag with UUID ";
        // for (uint8_t i = 0; i < uidlen; i++) {
        //     std::cout << std::hex << uid[i] ;
        //     std::cout << std::dec;
        //     std::cout << " ";
        // }
        std::cout << "Found a tag with UUID ";
        for (size_t i = 0; i < uidlen; ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(uid[i]) << ' ';
        }
        std::cout << std::dec << std::endl;
        std::cout << "" << std::endl;
        // if (uidlen == 4) {
        //     /* Assume Mifare Classic/Plus and set the global/default key. */
        //     reader.mifareLoadKey(reader.mifareKeyGlobal);
        //     /* Try to authenticate sectors 0..15. */
        //     for (uint8_t s = 0; s < 16; s++) {
        //         /* Try to authenticate this sector. */
        //         std::cout << "Sector " << s << std::endl;
        //         if(reader.mifareAuth(MIFARE_CMD_AUTH_A, s*4, uid)) {
        //             /* We should be able to read the sector contents now. */
        //             radio_mifare_dump_sector(reader, s);
        //         } else {
        //             std::cout << "AUTH_A failed for sector ";
        //             std::cout << s << std::endl;
        //         }
        //     }
        //     rc = true;
        // } else {
        //     std::cout << "Unexpected UID length: " << uidlen << std::endl;
        //     rc = false;
        // }
    } else {
        /* No tag found! */
        rc = false;
    }

    return rc;
}