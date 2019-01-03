// RGB Panel GFX Demo example for 16x32 panel
// By Marc MERLIN <marc_soft@merlins.org>
// Contains code (c) Adafruit, license BSD

// WILL NOT FIT on ARDUINO UNO -- requires a Mega, M0 or M4 board

#include <RGBmatrixPanel.h>
#include "test8x8.h"

// Most of the signal pins are configurable, but the CLK pin has some
// special constraints.  On 8-bit AVR boards it must be on PORTB...
// Pin 8 works on the Arduino Uno & compatibles (e.g. Adafruit Metro),
// Pin 11 works on the Arduino Mega.  On 32-bit SAMD boards it must be
// on the same PORT as the RGB data pins (D2-D7)...
// Pin 8 works on the Adafruit Metro M0 or Arduino Zero,
// Pin A4 works on the Adafruit Metro M4 (if using the Adafruit RGB
// Matrix Shield, cut trace between CLK pads and run a wire to A4).

//#define CLK  8 // USE THIS ON ARDUINO UNO, ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
#define CLK 11   // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

// Enable double buffering
RGBmatrixPanel *matrix = new RGBmatrixPanel(A, B, C, D, CLK, LAT, OE, true,64);

// Panel Matrix doesn't fully work like Neomatrix (which I originally
// wrote this demo for), so map a few calls to be compatible. The rest
// comes from Adafruit_GFX and works the same on both backends.
#define setBrightness(x) fillScreen(0) // no-op, no brightness on this board
#define clear()          fillScreen(0)
#define show()           swapBuffers(true)
#define Color(x,y,z)     Color444(x/32,y/32,z/32)

// Define matrix width and height.
#define mw 64
#define mh 32

// This could also be defined as matrix->color(255,0,0) but those defines
// are meant to work for Adafruit::GFX backends that are lacking color()
#define LED_BLACK           0

#define LED_RED_VERYLOW    (3 <<  11)
#define LED_RED_LOW        (7 <<  11)
#define LED_RED_MEDIUM     (15 << 11)
#define LED_RED_HIGH       (31 << 11)

#define LED_GREEN_VERYLOW  (1 <<  5)
#define LED_GREEN_LOW      (15 << 5)
#define LED_GREEN_MEDIUM   (31 << 5)
#define LED_GREEN_HIGH     (63 << 5)

#define LED_BLUE_VERYLOW     3
#define LED_BLUE_LOW         7
#define LED_BLUE_MEDIUM     15
#define LED_BLUE_HIGH       31

#define LED_ORANGE_VERYLOW (LED_RED_VERYLOW + LED_GREEN_VERYLOW)
#define LED_ORANGE_LOW     (LED_RED_LOW     + LED_GREEN_LOW)
#define LED_ORANGE_MEDIUM  (LED_RED_MEDIUM  + LED_GREEN_MEDIUM)
#define LED_ORANGE_HIGH    (LED_RED_HIGH    + LED_GREEN_HIGH)

#define LED_PURPLE_VERYLOW (LED_RED_VERYLOW + LED_BLUE_VERYLOW)
#define LED_PURPLE_LOW     (LED_RED_LOW     + LED_BLUE_LOW)
#define LED_PURPLE_MEDIUM  (LED_RED_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_PURPLE_HIGH    (LED_RED_HIGH    + LED_BLUE_HIGH)

#define LED_CYAN_VERYLOW   (LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_CYAN_LOW       (LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_CYAN_MEDIUM    (LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_CYAN_HIGH      (LED_GREEN_HIGH    + LED_BLUE_HIGH)

#define LED_WHITE_VERYLOW  (LED_RED_VERYLOW + LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_WHITE_LOW      (LED_RED_LOW     + LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_WHITE_MEDIUM   (LED_RED_MEDIUM  + LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_WHITE_HIGH     (LED_RED_HIGH    + LED_GREEN_HIGH    + LED_BLUE_HIGH)

void setup() {
    Serial.begin(115200);
    Serial.println("Setup");
    matrix->begin();
    matrix->setTextWrap(false);
    //matrix->setBrightness(BRIGHTNESS);
    //matrix->setBrightness(1);
    // Test full bright of all LEDs. If brightness is too high
    // for your current limit (i.e. USB), decrease it.
    matrix->fillScreen(LED_GREEN_LOW);
    matrix->show();
    delay(2000);
    matrix->clear();
    for (int i=0; i<2; i++) {
      matrix->clear();
      //fixdrawRGBBitmap1(10*(i+1),10*(i+1), (const uint32_t *)test8x8,8,8,i);
      matrix->show();
      //delay(2000);
    }
}


void loop() {
    // clear the screen after X bitmaps have been displayed and we
    // loop back to the top left corner
    // 8x8 => 1, 16x8 => 2, 17x9 => 6
    static uint8_t pixmap_count = ((mw+7)/8) * ((mh+7)/8);

    Serial.print("Screen pixmap capacity: ");
    Serial.println(pixmap_count);

matrix->clear();
matrix->drawLine(8,0, 8,25, LED_RED_VERYLOW);
matrix->drawLine(17,0, 17,25, LED_RED_VERYLOW);
matrix->drawLine(26,0, 26,25, LED_RED_VERYLOW);
matrix->drawLine(35,0, 35,25, LED_RED_VERYLOW);
matrix->drawLine(44,0, 44,25, LED_RED_VERYLOW);
 fixdrawRGBBitmap1(0,0,8,8,0);
 fixdrawRGBBitmap1(9,0,8,8,1);
 fixdrawRGBBitmap1(18,0,8,8,2);
 fixdrawRGBBitmap1(27,0,8,8,3);
 fixdrawRGBBitmap1(36,0,8,8,4);
 fixdrawRGBBitmap1(0,9,8,8,5);
 fixdrawRGBBitmap1(9,9,8,8,6);
 fixdrawRGBBitmap1(18,9,8,8,7);
 fixdrawRGBBitmap1(27,9,8,8,8);
 fixdrawRGBBitmap1(36,9,8,8,9);
 fixdrawRGBBitmap1(0,18,8,8,10);
 fixdrawRGBBitmap1(9,18,8,8,11);

 matrix->show();
 delay(10000);
  
}


// Convert a BGR 8/8/8 bitmap to RGB 5/6/5 used by Adafruit_GFX
void fixdrawRGBBitmap1(int16_t x, int16_t y, int16_t w, int16_t h, int pic) {
    uint16_t RGB_bmp_fixed[w * h];
    Serial.println("{");
    for (int j=0;j<64; j++) {
        uint32_t r,g,b;
        uint32_t color = pgm_read_dword_far(&(test8x8[pic][j]));
        b = (color & 0xFF0000) >> 16;
        g = (color & 0x00FF00) >> 8;
        r = color & 0x0000FF;
        // compress from 8/8/8 bits per color to 5/6/5
        b = map(b, 0, 255, 0, 31);
        g = map(g, 0, 255, 0, 63);
        r = map(r, 0, 255, 0, 31);
        RGB_bmp_fixed[j] = (r << 11) + (g << 5) + b;
        Serial.print("0x");
        Serial.print(RGB_bmp_fixed[j], HEX);
        if ((j+1)%64 != 0)Serial.print(",");
        if ((j+1)%8 == 0) Serial.println("");
        if ((j+1)%64 == 0) Serial.println("},");
    }
    matrix->drawRGBBitmap(x, y, RGB_bmp_fixed, w, h);
}


// Fill the screen with multiple levels of white to gauge the quality
void display_four_white() {
    matrix->clear();
    matrix->fillRect(0,0, mw,mh, LED_WHITE_HIGH);
    matrix->drawRect(1,1, mw-2,mh-2, LED_WHITE_MEDIUM);
    matrix->drawRect(2,2, mw-4,mh-4, LED_WHITE_LOW);
    matrix->drawRect(3,3, mw-6,mh-6, LED_WHITE_VERYLOW);
    matrix->show();
}


void display_lines() {
    matrix->clear();

    // 4 levels of crossing red lines.
    matrix->drawLine(0,mh/2-2, mw-1,2, LED_RED_VERYLOW);
    matrix->drawLine(0,mh/2-1, mw-1,3, LED_RED_LOW);
    matrix->drawLine(0,mh/2,   mw-1,mh/2, LED_RED_MEDIUM);
    matrix->drawLine(0,mh/2+1, mw-1,mh/2+1, LED_RED_HIGH);

    // 4 levels of crossing green lines.
    matrix->drawLine(mw/2-2, 0, mw/2-2, mh-1, LED_GREEN_VERYLOW);
    matrix->drawLine(mw/2-1, 0, mw/2-1, mh-1, LED_GREEN_LOW);
    matrix->drawLine(mw/2+0, 0, mw/2+0, mh-1, LED_GREEN_MEDIUM);
    matrix->drawLine(mw/2+1, 0, mw/2+1, mh-1, LED_GREEN_HIGH);

    // Diagonal blue line.
    matrix->drawLine(0,0, mw-1,mh-1, LED_BLUE_HIGH);
    matrix->drawLine(0,mh-1, mw-1,0, LED_ORANGE_MEDIUM);
    matrix->show();
}

void display_boxes() {
    matrix->clear();
    matrix->drawRect(0,0, mw,mh, LED_BLUE_HIGH);
    matrix->drawRect(1,1, mw-2,mh-2, LED_GREEN_MEDIUM);
    matrix->fillRect(2,2, mw-4,mh-4, LED_RED_HIGH);
    matrix->fillRect(3,3, mw-6,mh-6, LED_ORANGE_MEDIUM);
    matrix->show();
}

void display_circles() {
    matrix->clear();
    matrix->drawCircle(mw/2,mh/2, 2, LED_RED_MEDIUM);
    matrix->drawCircle(mw/2-1-min(mw,mh)/8, mh/2-1-min(mw,mh)/8, min(mw,mh)/4, LED_BLUE_HIGH);
    matrix->drawCircle(mw/2+1+min(mw,mh)/8, mh/2+1+min(mw,mh)/8, min(mw,mh)/4, LED_ORANGE_MEDIUM);
    matrix->drawCircle(1,mh-2, 1, LED_GREEN_LOW);
    matrix->drawCircle(mw-2,1, 1, LED_GREEN_HIGH);
    matrix->show();
}

void display_resolution() {
    // not wide enough;
    if (mw<16) return;
    matrix->clear();
    // Font is 5x7, if display is too small
    // 8 can only display 1 char
    // 16 can almost display 3 chars
    // 24 can display 4 chars
    // 32 can display 5 chars
    matrix->setCursor(0, 0);
    matrix->setTextColor(matrix->Color(255,0,0));
    if (mw>10) matrix->print(mw/10);
    matrix->setTextColor(matrix->Color(255,128,0));
    matrix->print(mw % 10);
    matrix->setTextColor(matrix->Color(0,255,0));
    matrix->print('x');
    // not wide enough to print 5 chars, go to next line
    if (mw<25) {
        if (mh==13) matrix->setCursor(6, 7);
        else if (mh>=13) {
            matrix->setCursor(mw-11, 8);
        } else {
            matrix->show();
            delay(2000);
            matrix->clear();
            matrix->setCursor(mw-11, 0);
        }
    }
    matrix->setTextColor(matrix->Color(0,255,128));
    matrix->print(mh/10);
    matrix->setTextColor(matrix->Color(0,128,255));
    matrix->print(mh % 10);
    // enough room for a 2nd line
    if (mw>25 && mh >14 || mh>16) {
        matrix->setCursor(0, mh-7);
        matrix->setTextColor(matrix->Color(0,255,255));
        if (mw>16) matrix->print('*');
        matrix->setTextColor(matrix->Color(255,0,0));
        matrix->print('R');
        matrix->setTextColor(matrix->Color(0,255,0));
        matrix->print('G');
        matrix->setTextColor(matrix->Color(0,0,255));
        matrix->print("B");
        matrix->setTextColor(matrix->Color(255,255,0));
        matrix->print("*");
    }

    matrix->show();
}

void display_scrollText() {
    matrix->clear();
    matrix->setTextWrap(false);  // we don't wrap text so it scrolls nicely
    matrix->setTextSize(1);
    matrix->setRotation(0);
    for (int8_t x=7; x>=-42; x--) {
        matrix->clear();
        matrix->setCursor(x,0);
        matrix->setTextColor(LED_GREEN_HIGH);
        matrix->print("Hello");
        if (mh>11) {
            matrix->setCursor(-20-x,mh-7);
            matrix->setTextColor(LED_ORANGE_HIGH);
            matrix->print("World");
        }
        matrix->show();
        delay(50);
    }

    matrix->setRotation(3);
    matrix->setTextColor(LED_BLUE_HIGH);
    for (int8_t x=7; x>=-45; x--) {
        matrix->clear();
        matrix->setCursor(x,mw/2-4);
        matrix->print("Rotate");
        matrix->show();
        delay(50);
    }
    matrix->setRotation(0);
    matrix->setCursor(0,0);
    matrix->show();
}







