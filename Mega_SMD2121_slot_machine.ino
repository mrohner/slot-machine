// WILL NOT FIT on ARDUINO UNO -- requires a Mega, M0 or M4 board

#include <RGBmatrixPanel.h>
#include "test8x8reduced.h"
#include "font5x5.h"
/*
Arduino Mega pins:
D3: Play button
D4: Bet_down button
D5: Bet_up button
D6: Buzzer over 10 Ohm resistor
D9: OE SMD2121
D10: LAT SMD2121
D11: CLK SMD2121
D21: Coin selector
A0: A SMD2121
A1: B SMD2121
A2: C SMD2121
A3: D SMD2121
A8: Floating for random number generator
*/


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
#define clear()          fillScreen(0)
#define show()           swapBuffers(true)
#define pay_time()       PAYTABLE[m+counter-1]*bet[bet_index]/20

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
#define LED_PURPLE_MEDIUM     (LED_RED_LOW     + LED_BLUE_LOW)
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

// Definitions for buttons
const int Bet_up = 5;
const int Bet_down = 4;
const int Play_button = 3;

const byte CH[] ={11,11,11,10,10,10,2,2,2,2,0,0,0,0,3,3,3,3,3,4,4,4,4,4,
5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,8,8,8,8,8,8,8,8,8,8,
1,1,1,1,1,1,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,7,7,7,7,7,7,7,7,
7,7,7,7,7};// Probality depends on how many times a symbol is listed
const byte CHlength = 85;
byte wild_symbol = 10;
byte scatter_symbol =11;

const byte PAYLINES[20][5] = {// 21 = 2nd row 1st char
  {21,22,23,24,25},
  {11,12,13,14,15},
  {31,32,33,34,35},
  {11,22,33,24,15},
  {31,22,13,24,35},
  {11,12,23,14,15},
  {31,32,23,34,35},
  {21,32,33,34,25},
  {21,12,13,14,25},
  {21,12,23,14,25},
  {21,32,23,34,25},
  {11,22,13,24,15},
  {31,22,33,24,35},
  {21,22,13,24,25},
  {21,22,33,24,25},
  {11,22,23,24,15},
  {31,22,23,24,35},
  {11,22,33,34,35},
  {31,22,13,14,15},
  {11,32,13,34,15}
};
byte win_per_payline[20];
const byte PAYTABLE []={0,0,20,100,250,  // Cocktail
                        2,0,20,100,250,  // Aperol
                        3,0,15,75,200,   // Drink
                        4,0,15,75,200,   // Beer
                        5,0,15,75,200,   // Orangina
                        10,5,50,250,1000,// (wild) rocket
                        6,0,5,50,100,    // Strawberry
                        8,0,5,50,100,    // Aubergine
                        1,0,5,50,100,    // Corn
                        7,0,2,15,75,     // Räbe
                        9,0,2,15,75,     // Radish
                        11,0,0,0,0};     // (scatter) blue thing
byte draw [3][5];
long randNumber;
int scatter = 0;
volatile int total = 500; // Start money
int win = 0;
int free_spins = 0;
bool free_spinning = false;
int free_spin_win = 0;
const int bet[] = {20,40,100,200,500,1000};
int bet_index = 0;
byte wild_counter = 0;

bool validnumber (byte tobechecked, int line, int position) {
  for (int k = 0; k<line;k++) {
    if(tobechecked == draw [k] [position])return false;
  }
  return true;    
}


// Definitions for coin acceptor
const int COINPIN = 21;

// Buzzer
const int speakerOut = 6;


void setup() {
  Serial.begin(115200);
  Serial.println("Setup");
  pinMode(Bet_up,INPUT_PULLUP);
  pinMode(Bet_down,INPUT_PULLUP);
  pinMode(Play_button,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(COINPIN),coinInterrupt,RISING);
  randomSeed(analogRead(A8));
  matrix->begin();
  matrix->clear();
  draw_red_lines();
  spin(5,1,false);
  display_amounts();
  matrix->show(); 
}


void loop() {
  while (total < 20 && !free_spinning){
    coin_please();
  }
  do {
    display_amounts();
    drawText(0,26,"         ", 9,LED_BLACK);
    matrix->show();
    if (digitalRead(Bet_up)==LOW){
      bet_index--;
      if (bet_index<0) bet_index=0;
      display_amounts();
      matrix->show();
      delay(300);
    }
    if (digitalRead(Bet_down)==LOW){
      bet_index++;
      if (bet_index>5) bet_index=5;
      if (bet[bet_index]>total) bet_index--;
      display_amounts();
      matrix->show();
      delay(300);
    }
   } while (digitalRead(Play_button) && free_spinning == false);
  win = 0;
  randomSeed(analogRead(A8));
  if (free_spins >= 0 && free_spinning) {
    free_spins--;
    display_free_spins();
    matrix->show();
    if (free_spins == -1){
      delay(2000);
      win += free_spin_win;
      total += win;
      free_spin_win = 0;
      free_spinning = false;  
      display_amounts();
      matrix->show();
      delay(2000);
      drawText(0,26,"         ", 9,LED_BLACK);
      //matrix->show();
    }
  }
  else {
    total -= bet[bet_index];
    drawText(0,26,"         ", 9,LED_BLACK);
    matrix->show();
  }
  display_amounts();
  matrix->show();
  //if(free_spinning) delay(2000);
  Serial.println(F("---------------"));
  Serial.println(F("Begin"));
  spin(5,9,false);
  for (int j = 0; j<5;j++) {
    spin(4-j,1,false);
    check_scatter(4-j);
    if (scatter == 2 && j<4) {
      spin(4-j,40,true);
    }
    if (scatter == 3) break;
    if (j == 4) evaluate(4);
    //delay(200);
  }
  if (scatter < 3) {
    evaluate(5);
    total += win;
    if (win > (bet[bet_index]*10)) {
      drawText(8,26,"BIG WIN",7, LED_RED_HIGH);//x,y,with,hight,text,size of text,color
      matrix->show();
      delay(1000);
      drawText(0,26,"         ", 9,LED_BLACK);
    }
    display_amounts();
    matrix->show();
    display_paylines();
  }
  else {
    free_spins+=5;
    free_spinning=true;
    display_free_spins();
    matrix->show();
    delay(2000);
    }
  matrix->show();
  delay(200);
}


void spin(byte spins, byte number_spins, bool spin_for_scatters){
  for (int k = 0;k<number_spins;k++){//how many spins
    for (int j = 5-spins; j<5;j++) {
       for(int i = 0; i<3;i++) {
         do {
           randNumber = random(CHlength);
           //Serial.print("Random ");
           //Serial.println(CH[randNumber]);
            } while (!validnumber(CH[randNumber],i,j));
         draw [i][j]= CH[randNumber];
         drawBitmap1(j*9,i*9,8,8,CH[randNumber]);// x,y,with, hight, image number
         matrix->show();
         if (spin_for_scatters && CH[randNumber] == scatter_symbol) {
            scatter++;
            Serial.print(F("Scatter in Spin "));
            Serial.println(scatter);
           }
         
         //matrix->swapBuffers(false);
       }
       //delay(10);
       if (scatter == 3) break;
    }
    if (scatter == 3) break; 
  }
}


void check_scatter(int position){
  scatter = 0;
   for (int j = 0 ; j<(5-position);j++) {
       for(int i = 0; i<3;i++) {
        if (draw [i][j]== scatter_symbol) {
          scatter++;
          Serial.print(F("Scatter in Spin "));
          Serial.println(scatter);
        }
      }
   }
}


void evaluate(int how_many){
  //draw[0][0]=10;//for testing only
  Serial.print(F("Evaluate "));
  Serial.println(how_many);
  for (int i = 0; i<20;i++) {
    Serial.print(F("Payline: "));
    Serial.println(i+1);
    byte line [5];
    for (int j = 0; j<how_many;j++) { 
      line[j]=draw[PAYLINES[i][j]/10-1][PAYLINES[i][j]%10-1];
      //Serial.println(line[j]);  
     }
    byte counter = 1;
    byte first = line[0];
    byte next = 254;
    byte wild_counter = 1;
    if (first == wild_symbol)Serial.println(F("1st is Wild"));
    for (int j = 1; j < how_many;j++) {
      if (first == scatter_symbol) break; //if it is a scatter symbol
      if (first == wild_symbol) {
        Serial.print ("j: ");
        Serial.println(j);
        Serial.print ("line[j]: ");
        Serial.println(line[j]);
        if (line[j] == wild_symbol) {
          wild_counter++;
          counter++;
          Serial.print(F("another Wild: "));
          Serial.println(counter); 
        }
        else if (next == 254) {
          next = line[j];
          counter++;
          Serial.print(F("Next: "));
          Serial.println(next);
          Serial.print(F("CounterN: "));
          Serial.print(" ");
          Serial.println(counter); 
        }
        else if (next == line[j] || line[j] == wild_symbol) {
          counter++;
          Serial.print(F("CounterW: "));
          Serial.print(next);
          Serial.print(" ");
          Serial.println(counter); 
        }
        else {
          Serial.print(F("CounterWBits: "));
          Serial.print(next);
          Serial.print(" ");
          Serial.println(counter); 
          if (wild_counter <4 && wild_counter > 0 && next !=7 && next !=9) first=next;
          if ((next == 7 || next == 9) && counter >2 && first == wild_symbol) first=next;
          // three of 7 and 9 give less points than two wild cards
          // to be amended
          break;
        }
      }
      else if (line[j] == first || line[j] == wild_symbol) {
        counter++;
        Serial.print(F("Counter: "));
        Serial.print(first);
        Serial.print(" ");
        Serial.println(counter); 
        if (how_many == 4 && counter == 4){
          spin(1,40,false);
          break; 
        }
      }
      else break;
     }
     if (how_many == 4 && counter == 4)break;
     if (how_many == 5) {
      if (counter > 2 ){
        for (int m = 0; m<55;m+=5) {
          if (first == PAYTABLE[m]) {
            if (free_spinning) {
              free_spin_win += pay_time();
              win_per_payline[i] = pay_time();
              drawText(0,26,"         ", 9,LED_BLACK);
              display_paylines();
              drawText(0,26,"         ", 9,LED_BLACK);
              display_free_spins();
              matrix->show();
              break;
            }
            else {
              win += pay_time();
              Serial.print(F("Win: "));
              Serial.println(pay_time());
              win_per_payline[i] = pay_time();
              break;
            }
          }
        }
      }
    }
  }
}


void display_amounts(){
  drawText(45,0,"Tot", 3,LED_BLUE_HIGH);
  drawNumber(total,44,5,4,LED_PURPLE_MEDIUM); //number,x,y,number of characters
  drawText(44,10,"Bet", 3,LED_BLUE_HIGH);
  drawNumber(bet[bet_index],44,15,4,LED_PURPLE_MEDIUM);
  drawText(45,20,"Win", 3,LED_BLUE_HIGH);
  drawNumber(win,44,25,4,LED_PURPLE_MEDIUM);  
}


void drawBitmap1(int16_t x, int16_t y, int16_t w, int16_t h, int pic) {
    uint16_t RGB_bmp_fixed[w * h];
    for (int j=0;j<(w*h); j++) RGB_bmp_fixed[j] = pgm_read_dword_far(&(test8x8[pic][j]));
    matrix->drawRGBBitmap(x, y, RGB_bmp_fixed, w, h);
}


void drawText(int16_t x, int16_t y,char* text, int16_t size, uint16_t color) {
    for (int i=0;i<size;i++) {
      uint16_t RGB_bmp_fixed[25];
      uint16_t RGB_bmp_fixed_r[25];
      int ASCIIValue = text[i];
      for (int j=0;j<5; j++) {//Loop through the 5 bytes
        uint8_t fontline = pgm_read_byte_far(&(font5x5[(ASCIIValue-32)*5+4-j]));
         for (int k=0;k<5;k++) {//loop through the 5 bits
          RGB_bmp_fixed[5*j+k] = (bitRead(fontline,k) == 1)? color : LED_BLACK;
         }
      }
     //Rotate font 90 degrees
     for (uint8_t i = 0; i < 5; i++)
       for (uint8_t j = 0; j < 5; j++)
        RGB_bmp_fixed_r[(i*5)+j] = RGB_bmp_fixed[((4-j)*5)+i];
      matrix->drawRGBBitmap(x, y, RGB_bmp_fixed_r, 5, 5);
      x+=5;
  }
}


void drawNumber(int value,int x,int y,int chiffres, uint16_t color){
  String stringOne = "";
  if (chiffres > 3 &&(value/100)<10)  stringOne =  stringOne + ' ';
  if (chiffres > 2 &&(value/10)<10) stringOne =  stringOne + ' ';
  if (chiffres > 1 &&(value)<10) stringOne =  stringOne + ' ';
  stringOne =  stringOne + value;
  drawText(x,y,stringOne.c_str(),chiffres,color);
  matrix->drawLine(44,0, 44,25, LED_RED_VERYLOW);
}

void display_paylines(){
   for (int l = 0; l<20;l++) {
      if(win_per_payline[l]){
        display_line(l,LED_BLUE_HIGH);
        drawText(0,26,"Win ", 4,LED_BLUE_HIGH);
        drawNumber(win_per_payline[l],19,26,4,LED_RED_HIGH);
        matrix->show();
        delay(5000);
        drawText(0,26,"         ", 9,LED_BLACK);
        display_line(l,LED_BLACK);
        draw_board();
        draw_red_lines();
        matrix->show();
     }
   }
   for (int l = 0; l<20;l++) win_per_payline[l] = 0;
}


void display_line(int payline, uint16_t color ) {
   for (int i = 0; i < 4; i++){
    int starty = 3+((PAYLINES[payline][i]/10-1)*9);
    int startx = 3+((PAYLINES[payline][i]%10-1)*9);
    int desty = 3+((PAYLINES[payline][i+1]/10-1)*9);
    int destx = 3+((PAYLINES[payline][i+1]%10-1)*9);
    matrix->drawLine(startx,starty,destx,desty,color);
   }
}


void draw_board(){
   for (int i=0;i<3; i++) 
         for (int j=0;j<5;j++) drawBitmap1(j*9,i*9,8,8,draw[i][j]);// x,y,with, hight, image number;
}


void draw_red_lines(){
  matrix->drawLine(8,0, 8,25, LED_RED_VERYLOW);
  matrix->drawLine(17,0, 17,25, LED_RED_VERYLOW);
  matrix->drawLine(26,0, 26,25, LED_RED_VERYLOW);
  matrix->drawLine(35,0, 35,25, LED_RED_VERYLOW);
  matrix->drawLine(44,0, 44,25, LED_RED_VERYLOW);
}


void display_free_spins(){
  drawText(0,26,"F", 1,LED_BLUE_HIGH);
  drawNumber(free_spins,6,26,2,LED_PURPLE_MEDIUM); //number,x,y,number of characters
  drawText(17,26,"W",1,LED_BLUE_HIGH);
  drawNumber(free_spin_win,23,26,4,LED_PURPLE_MEDIUM);
}


void coin_please(){
  drawText(0,26,"COIN PLS.",9, LED_RED_HIGH);//x,y,with,hight,text,size of text,color
  matrix->show();
  delay(500);
}


void coinInterrupt(){
  // Each time a pulse is sent from the coin acceptor,
  // interrupt main loop to add 100 
  // Coins: 10Cents =1 pulse,20C=2,50C=5,1Fr=10,2Fr=20,5Fr=50 pulses
  total+=100;
}

