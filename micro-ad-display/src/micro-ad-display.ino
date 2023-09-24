/*
 * Project micro-ad-display
 * Description: Display GIF-based ads that can be updated OTA via Asset OTA on a Particle Photon 2 with metrics
 * Author: Evan Rust
 * Date: 24-09-2023
 */

#include <Particle.h>
#include <fcntl.h>
#include <SPI.h>
#include "Adafruit_mfGFX/Adafruit_mfGFX.h"
#include "Adafruit_SSD1351_Photon.h"
#include "Bitmap.h"

constexpr int BlockSize = 512;
constexpr uint8_t CsPin = D18;
constexpr uint8_t DcPin = D19;
constexpr uint8_t RstPin = D14;

void handleAssets(spark::Vector<ApplicationAsset> assets);

Adafruit_SSD1351 tft = Adafruit_SSD1351(CsPin, DcPin, RstPin);
Bitmap bitmap = Bitmap(&tft);
SerialLogHandler logHandler(LOG_LEVEL_TRACE);

void setup() {
    waitFor(Serial.isConnected, 10000); delay(2000);
    handleAssets(System.assetsAvailable());

    tft.begin();
    tft.fillScreen(0);
}

void loop() {
    // If ad is present, display it

    // If the button has been pressed, show the QR code

    // Check APDS9960 for proximity
}

void handleAssets(spark::Vector<ApplicationAsset> assets) {
    for (auto& asset : assets) {
        int size = (int)asset.size();
        String name = asset.name();
        if (name == "ad.gif" || name == "qrcode.bmp") {
            uint8_t buf[BlockSize];
            int32_t bytesRead = 0;
            int fd = open(name, O_WRONLY | O_CREAT | O_TRUNC);
            if (fd != -1) {
                while (bytesRead < size) {
                    int toRead = constrain(size - bytesRead, 0, sizeof(buf));
                    toRead = asset.read((char*)buf, toRead);

                    if (toRead <= 0) break;

                    bytesRead += toRead;

                    write(fd, buf, toRead);
                }

                close(fd);
            }
        }
    }

    System.assetsHandled(true);
}