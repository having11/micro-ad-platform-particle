/*
 * Project micro-ad-display
 * Description: Display GIF-based ads that can be updated OTA via Asset OTA on a Particle Photon 2 with metrics
 * Author: Evan Rust
 * Date: 24-09-2023
 */

#include <Particle.h>
#include <fcntl.h>
#include <dirent.h>
#include <SPI.h>
#include "Adafruit_mfGFX/Adafruit_mfGFX.h"
#include "Adafruit_SSD1351_Photon.h"
#include "Bitmap.h"
#include <vector>

constexpr int BlockSize = 512;
constexpr uint8_t CsPin = D18;
constexpr uint8_t DcPin = D19;
constexpr uint8_t RstPin = D14;
constexpr uint8_t ButtonPin = D4;
constexpr uint8_t PirPin = D5;
constexpr uint16_t AdSwitchDelayMs = 4000;

uint8_t currentAd = 0;
uint32_t lastAdDisplayMs = 0;
static std::vector<String> adFileNames;
volatile bool pirTriggered = false;
volatile bool buttonPress = false;

void handleAssets(spark::Vector<ApplicationAsset> assets);
void handlePir();
void handleButton();
void loadAdFilenames();

Adafruit_SSD1351 tft = Adafruit_SSD1351(CsPin, DcPin, RstPin);
Bitmap bitmap = Bitmap(&tft);
SerialLogHandler logHandler(LOG_LEVEL_INFO);

void setup() {
    Serial.begin(115200);
    waitFor(Serial.isConnected, 10000); delay(2000);

    Log.info("Starting assets available=%d", System.assetsAvailable().size());
    handleAssets(System.assetsAvailable());

    tft.begin();
    tft.fillCircle(60, 60, 20, 0x0ff0);
    delay(2000);
    tft.fillScreen(0xff00);
    delay(2000);

    pinMode(ButtonPin, INPUT_PULLUP);
    attachInterrupt(ButtonPin, handleButton, FALLING);
    pinMode(PirPin, INPUT);
    attachInterrupt(PirPin, handlePir, RISING);

    loadAdFilenames();
}

void loop() {
    // If ad is present, display it
    if (millis() - lastAdDisplayMs >= AdSwitchDelayMs) {
        Log.info("draw bitmap start");
        if (adFileNames.size() > 0) {
            bitmap.drawBitmap(adFileNames[currentAd++ % adFileNames.size()].c_str());
        }
        lastAdDisplayMs = millis();
    }

    // If the button has been pressed, show the QR code
    if (buttonPress) {
        Particle.publish("AD_BUTTON_PRESS");
        bitmap.drawBitmap("qrcode.bmp");

        delay(6000);
    }

    // Check PIR for proximity
    if (pirTriggered) {
        Particle.publish("AD_IMPRESSION");
    }
}

void loadAdFilenames() {
    struct stat statbuf;

    DIR* dir = opendir("/");
    Log.info("Opened dir=%d", errno);
    if (dir != NULL) {
        dirent *result = readdir(dir);
        while (result) {
            if (result->d_type == DT_REG) {
                adFileNames.push_back(result->d_name);
            }
            result = readdir(dir);
        }

        closedir(dir);
        Log.info("Closed dir");
    }
}

void handlePir() {
    pirTriggered = true;
}

void handleButton() {
    buttonPress = true;
}

void handleAssets(spark::Vector<ApplicationAsset> assets) {
    // Delete all previous ad files
    loadAdFilenames();
    Log.info("ad file names length = %d", adFileNames.size());
    for (auto& path : adFileNames) {
        int result = unlink(path.c_str());
        if (result != 0) {
            // Error
        }
    }

    for (auto& asset : assets) {
        int size = (int)asset.size();
        String name = asset.name();
        Log.info("Found asset %s", name.c_str());
        if (name.startsWith("ad") || name == "qrcode.bmp") {
            uint8_t buf[BlockSize];
            int32_t bytesRead = 0;
            int fd = open(name, O_WRONLY | O_CREAT | O_TRUNC);
            Log.info("Got fd=%d", fd);
            if (fd != -1) {
                while (bytesRead < size) {
                    int toRead = constrain(size - bytesRead, 0, sizeof(buf));
                    toRead = asset.read((char*)buf, toRead);

                    if (toRead <= 0) break;

                    bytesRead += toRead;

                    write(fd, buf, toRead);
                }

                close(fd);
                Log.info("Closed fd=%d", fd);
            }
        }
    }

    System.assetsHandled(true);
}