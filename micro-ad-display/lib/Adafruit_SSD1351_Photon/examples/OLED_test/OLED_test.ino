/*************************************************** 
  This is a example sketch demonstrating graphic drawing
  capabilities of the SSD1351 library for the 1.5" 
  and 1.27" 16-bit Color OLEDs with SSD1351 driver chip

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/1431
  ------> http://www.adafruit.com/products/1673
 
  If you're using a 1.27" OLED, change SSD1351HEIGHT in Adafruit_SSD1351.h
 	to 96 instead of 128

  These displays use SPI to communicate, 4 or 5 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution

  The Adafruit GFX Graphics core library is also required
  https://github.com/adafruit/Adafruit-GFX-Library
  Be sure to install it!
    
  ----------------

  Modified by Mike Heininger and Nathan Friedly for compatibility with 
  the Particle Photon & Particle Build IDE.
 ****************************************************/

// You can use any 5 pins; see note below about hardware vs software SPI
#define cs   D18
#define rst  D14
#define dc   D19

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

#include "Adafruit_mfGFX/Adafruit_mfGFX.h"
#include "Adafruit_SSD1351_Photon.h"

// Option 1: Hardware SPI - uses some analog pins, but much faster
Adafruit_SSD1351 tft = Adafruit_SSD1351(cs, dc, rst);

// Option 2: Software SPI - use any pins but a little slower
//Adafruit_SSD1351 tft = Adafruit_SSD1351(cs, dc, mosi, sclk, rst);  

float p = 3.1415926;

void fillpixelbypixel(uint16_t color) {
  for (uint8_t x=0; x < tft.width(); x++) {
    for (uint8_t y=0; y < tft.height(); y++) {
      tft.drawPixel(x, y, color);
    }
  }
  delay(100);
}

void setup(void) {
  Serial.begin(9600);
  Serial.print("hello!");
  tft.begin();

  Serial.println("init");
}

void loop() {
  // You can optionally rotate the display by running the line below.
  // Note that a value of 0 means no rotation, 1 means 90 clockwise,
  // 2 means 180 degrees clockwise, and 3 means 270 degrees clockwise.
  //tft.setRotation(1);
  // NOTE: The test pattern at the start will NOT be rotated!  The code
  // for rendering the test pattern talks directly to the display and
  // ignores any rotation.

  uint16_t time = millis();
  tft.fillRect(0, 0, 128, 128, BLACK);
  time = millis() - time;
  
  Serial.println(time, DEC);
  delay(500);
  
  lcdTestPattern();
  delay(500);
  
  tft.invert(true);
  delay(100);
  tft.invert(false);
  delay(100);

  tft.fillScreen(BLACK);
  testdrawtext("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", WHITE);
  delay(500);

  // tft print function!
  tftPrintTest();
  delay(500);
  
  //a single pixel
  tft.drawPixel(tft.width()/2, tft.height()/2, GREEN);
  delay(500);

  // line draw test
  testlines(YELLOW);
  delay(500);    
 
  // optimized lines
  testfastlines(RED, BLUE);
  delay(500);    


  testdrawrects(GREEN);
  delay(1000);

  testfillrects(YELLOW, MAGENTA);
  delay(1000);

  tft.fillScreen(BLACK);
  testfillcircles(10, BLUE);
  testdrawcircles(10, WHITE);
  delay(1000);
   
  testroundrects();
  delay(500);
  
  testtriangles();
  delay(500);
  
  Serial.println("done");
  delay(1000);

}

void testlines(uint16_t color) {
   tft.fillScreen(BLACK);
   for (uint16_t x=0; x < tft.width()-1; x+=6) {
     tft.drawLine(0, 0, x, tft.height()-1, color);
   }
   for (uint16_t y=0; y < tft.height()-1; y+=6) {
     tft.drawLine(0, 0, tft.width()-1, y, color);
   }
   
   tft.fillScreen(BLACK);
   for (uint16_t x=0; x < tft.width()-1; x+=6) {
     tft.drawLine(tft.width()-1, 0, x, tft.height()-1, color);
   }
   for (uint16_t y=0; y < tft.height()-1; y+=6) {
     tft.drawLine(tft.width()-1, 0, 0, y, color);
   }
   
   tft.fillScreen(BLACK);
   for (uint16_t x=0; x < tft.width()-1; x+=6) {
     tft.drawLine(0, tft.height()-1, x, 0, color);
   }
   for (uint16_t y=0; y < tft.height()-1; y+=6) {
     tft.drawLine(0, tft.height()-1, tft.width()-1, y, color);
   }

   tft.fillScreen(BLACK);
   for (uint16_t x=0; x < tft.width()-1; x+=6) {
     tft.drawLine(tft.width()-1, tft.height()-1, x, 0, color);
   }
   for (uint16_t y=0; y < tft.height()-1; y+=6) {
     tft.drawLine(tft.width()-1, tft.height()-1, 0, y, color);
   }
   
}

void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0,0);
  tft.setTextColor(color);
  tft.print(text);
}

void testfastlines(uint16_t color1, uint16_t color2) {
   tft.fillScreen(BLACK);
   for (uint16_t y=0; y < tft.height()-1; y+=5) {
     tft.drawFastHLine(0, y, tft.width()-1, color1);
   }
   for (uint16_t x=0; x < tft.width()-1; x+=5) {
     tft.drawFastVLine(x, 0, tft.height()-1, color2);
   }
}

void testdrawrects(uint16_t color) {
 tft.fillScreen(BLACK);
 for (uint16_t x=0; x < tft.height()-1; x+=6) {
   tft.drawRect((tft.width()-1)/2 -x/2, (tft.height()-1)/2 -x/2 , x, x, color);
 }
}

void testfillrects(uint16_t color1, uint16_t color2) {
 tft.fillScreen(BLACK);
 for (uint16_t x=tft.height()-1; x > 6; x-=6) {
   tft.fillRect((tft.width()-1)/2 -x/2, (tft.height()-1)/2 -x/2 , x, x, color1);
   tft.drawRect((tft.width()-1)/2 -x/2, (tft.height()-1)/2 -x/2 , x, x, color2);
 }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (uint8_t x=radius; x < tft.width()-1; x+=radius*2) {
    for (uint8_t y=radius; y < tft.height()-1; y+=radius*2) {
      tft.fillCircle(x, y, radius, color);
    }
  }  
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (uint8_t x=0; x < tft.width()-1+radius; x+=radius*2) {
    for (uint8_t y=0; y < tft.height()-1+radius; y+=radius*2) {
      tft.drawCircle(x, y, radius, color);
    }
  }  
}

void testtriangles() {
  tft.fillScreen(BLACK);
  int color = 0xF800;
  int t;
  int w = tft.width()/2;
  int x = tft.height();
  int y = 0;
  int z = tft.width();
  for(t = 0 ; t <= 15; t+=1) {
    tft.drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
  }
}

void testroundrects() {
  tft.fillScreen(BLACK);
  int color = 100;
  
  int x = 0;
  int y = 0;
  int w = tft.width();
  int h = tft.height();
  for(int i = 0 ; i <= 24; i++) {
    tft.drawRoundRect(x, y, w, h, 5, color);
    x+=2;
    y+=3;
    w-=4;
    h-=6;
    color+=1100;
    Serial.println(i);
  }
}

void tftPrintTest() {
  tft.fillScreen(BLACK);
  tft.setCursor(0, 5);
  tft.setTextColor(RED);  
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(YELLOW);
  tft.setTextSize(2);
  tft.println("Hello World!");
  tft.setTextColor(BLUE);
  tft.setTextSize(3);
  tft.print(1234.567);
  delay(1500);
  tft.setCursor(0, 5);
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(0);
  tft.println("Hello World!");
  tft.setTextSize(1);
  tft.setTextColor(GREEN);
  tft.print(p, 6);
  tft.println(" Want pi?");
  tft.println(" ");
  tft.print(8675309, HEX); // print 8,675,309 out in HEX!
  tft.println(" Print HEX!");
  tft.println(" ");
  tft.setTextColor(WHITE);
  tft.println("Sketch has been");
  tft.println("running for: ");
  tft.setTextColor(MAGENTA);
  tft.print(millis() / 1000);
  tft.setTextColor(WHITE);
  tft.print(" seconds.");
}

void mediabuttons() {
 // play
  tft.fillScreen(BLACK);
  tft.fillRoundRect(25, 10, 78, 60, 8, WHITE);
  tft.fillTriangle(42, 20, 42, 60, 90, 40, RED);
  delay(500);
  // pause
  tft.fillRoundRect(25, 90, 78, 60, 8, WHITE);
  tft.fillRoundRect(39, 98, 20, 45, 5, GREEN);
  tft.fillRoundRect(69, 98, 20, 45, 5, GREEN);
  delay(500);
  // play color
  tft.fillTriangle(42, 20, 42, 60, 90, 40, BLUE);
  delay(50);
  // pause color
  tft.fillRoundRect(39, 98, 20, 45, 5, RED);
  tft.fillRoundRect(69, 98, 20, 45, 5, RED);
  // play color
  tft.fillTriangle(42, 20, 42, 60, 90, 40, GREEN);
}

/**************************************************************************/
/*! 
    @brief  Renders a simple test pattern on the LCD
*/
/**************************************************************************/
void lcdTestPattern(void)
{
  uint32_t i,j;
  tft.goTo(0, 0);
  
  for(i=0;i<128;i++)
  {
    for(j=0;j<128;j++)
    {
      if(i<16){
        tft.writeData(RED>>8); tft.writeData(RED);
      }
      else if(i<32) {
        tft.writeData(YELLOW>>8);tft.writeData(YELLOW);
      }
      else if(i<48){tft.writeData(GREEN>>8);tft.writeData(GREEN);}
      else if(i<64){tft.writeData(CYAN>>8);tft.writeData(CYAN);}
      else if(i<80){tft.writeData(BLUE>>8);tft.writeData(BLUE);}
      else if(i<96){tft.writeData(MAGENTA>>8);tft.writeData(MAGENTA);}
      else if(i<112){tft.writeData(BLACK>>8);tft.writeData(BLACK);}
      else {
        tft.writeData(WHITE>>8);      
        tft.writeData(WHITE);
       }
    }
  }
}
