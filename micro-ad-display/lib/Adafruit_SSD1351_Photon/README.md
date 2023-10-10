# Adafruit SSD1351 1.5" Color OLED library for Particle Photon

Connect an [Adafruit SSD1351 1.5" Color OLED](https://www.adafruit.com/products/1431) breakout board to a [Particle Photon](https://www.particle.io/products/hardware/photon-wifi-dev-kit). 

![Particle Photon with OLED Display](http://nfriedly.github.io/Adafruit_SSD1351_Photon/photon-oled.jpg)

## Limitations

### MicroSD Card is not currently supported
Should be possible, but I haven't yet gotten it working. See https://community.particle.io/t/particle-photon-oled-screen-and-sd-card-demo/18145 and https://github.com/mikeseeh/photon-demo-oled-sdcard for examples where folks have succeeded.

### Requires modification for other screen sizes

With a [small change](https://github.com/nfriedly/Adafruit_SSD1351_Photon/blob/master/src/Adafruit_SSD1351_Photon.h#L29), this library could be adjusted to work with [1.27 inch OLED](https://www.adafruit.com/products/684), and possibly the [0.96 inch OLED](https://www.adafruit.com/products/684) breakout boards.

## Wiring

For hardware SPI (considerably faster), connect the pins as follows:
    
    Photon  -> OLED
    ----------------------
    3V3     -> 3Vo (3V)
    GND     -> GND (G)
    
    D6      -> DC
    D5      -> RESET (R)
    
    A5      -> MOSI (SI)
    A3      -> SCK (CL)
    
    A2      -> OLEDCS (OC)

DC and RESET may be set to any pin, but the example defaults to D5 & D6.

Any combination of pins may be used for software SPI, but the display will update more slowly.

## API documentation

See https://learn.adafruit.com/adafruit-gfx-graphics-library/


## Credits

This is based on a collection of work from other tallented developers, 99.9% of the credit goes to them. 

* The lion's share of the credit goes to Adafruit - https://github.com/adafruit/Adafruit-SSD1351-library
* I started with https://github.com/mikeseeh/photon-demo-oled-sdcard
* Which I think is based on https://github.com/pkourany/Adafruit_SSD1351_Library/
