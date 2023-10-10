#pragma once

#include <Particle.h>
#include <fcntl.h>
#include "Adafruit_mfGFX/Adafruit_mfGFX.h"
#include "Adafruit_SSD1351_Photon.h"

class Bitmap {
    public:
        Bitmap(Adafruit_SSD1351 *tft) : _tft(tft) { }

        void drawBitmap(const char* filename);

    private:
        uint16_t read16(int fd);
        uint32_t read32(int fd);

        Adafruit_SSD1351 *_tft;
};
