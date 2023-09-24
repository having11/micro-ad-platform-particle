// See https://github.com/berryelectronics/Adafruit_SSD1351-STM32/blob/master/examples/bmp/bmp.ino

#include "Bitmap.h"

constexpr uint8_t BUF_SIZE = 32;

void Bitmap::drawBitmap(const char* filename) {
    uint8_t  sdbuffer[3 * BUF_SIZE]; // pixel buffer (R+G+B per pixel)
    uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        // error
        return;
    }

    // Parse BMP header
    if (read16(fd) == 0x4D42) { // BMP signature
        uint32_t fileSize = read32(fd);
        lseek(fd, 4, SEEK_CUR); // ignore creator bytes
        uint32_t bmpImageoffset = read32(fd); // Start of image data
        uint32_t headerSize = read32(fd);
        int bmpWidth  = read32(fd);
        int bmpHeight = read32(fd);
        uint16_t planeCount = read16(fd);
        if (planeCount == 1) { // # planes -- must be '1'
            uint8_t bmpDepth = read16(fd); // bits per pixel
            uint32_t compression = read32(fd);
            if ((bmpDepth == 24) && (compression == 0)) { // 0 = uncompressed
                bool flip = true;
                // BMP rows are padded (if needed) to 4-byte boundary
                uint32_t rowSize = (bmpWidth * 3 + 3) & ~3;

                // If bmpHeight is negative, image is in top-down order.
                // This is not canon but has been observed in the wild.
                if(bmpHeight < 0) {
                    bmpHeight = -bmpHeight;
                    flip = false;
                }

                // Crop area to be loaded
                int w = bmpWidth;
                int h = bmpHeight;

                if ((w - 1) >= _tft->width())  w = _tft->width();
                if ((h - 1) >= _tft->height()) h = _tft->height();

                for (int row = 0; row < h; row++) { // For each scanline...
                    uint32_t pos;
                    _tft->goTo(0, row);

                    // Seek to start of scan line.  It might seem labor-
                    // intensive to be doing this on every line, but this
                    // method covers a lot of gritty details like cropping
                    // and scanline padding.  Also, the seek only takes
                    // place if the file position actually needs to change
                    // (avoids a lot of cluster math in SD library).
                    if (flip) // Bitmap is stored bottom-to-top order (normal BMP)
                        pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
                    else     // Bitmap is stored top-to-bottom
                        pos = bmpImageoffset + row * rowSize;

                    lseek(fd, pos, SEEK_SET);
                    buffidx = sizeof(sdbuffer); // Force buffer reload

                    // optimize by setting pins now
                    for (int col = 0; col < w; col++) { // For each pixel...
                        // Time to read more pixel data?
                        if (buffidx >= sizeof(sdbuffer)) {
                            read(fd, sdbuffer, sizeof(sdbuffer));
                            buffidx = 0; // Set index to beginning
                        }

                        // Convert pixel from BMP to TFT format, push to display
                        uint8_t b = sdbuffer[buffidx++];
                        uint8_t g = sdbuffer[buffidx++];
                        uint8_t r = sdbuffer[buffidx++];

                        _tft->drawPixel(col, row, _tft->Color565(r, g, b));
                    } // end pixel
                } // end scanline
            } // end goodBmp
        }
    }

    close(fd);
}

uint16_t Bitmap::read16(int fd) {
  uint16_t result;
  int count = read(fd, &result, sizeof(result));
  return result;
}

uint32_t Bitmap::read32(int fd) {
  uint32_t result;
  int count = read(fd, &result, sizeof(result));
  return result;
}