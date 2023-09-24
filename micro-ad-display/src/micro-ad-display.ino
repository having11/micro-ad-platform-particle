/*
 * Project micro-ad-display
 * Description: Display GIF-based ads that can be updated OTA via Asset OTA on a Particle Photon 2 with metrics
 * Author: Evan Rust
 * Date: 24-09-2023
 */

#include <Particle.h>

void handleAssets(spark::Vector<ApplicationAsset> assets);

void setup() {
    auto assets = System.assetsAvailable();
}

void loop() {

}

void handleAssets(spark::Vector<ApplicationAsset> assets) {

}