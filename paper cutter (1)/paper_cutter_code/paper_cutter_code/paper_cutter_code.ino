
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#define I2C_ADDRESS 0x3C
#define RST_PIN -1
SSD1306AsciiWire oled;



int up = 10;
int down = 11;
int right = 13;
int left = 9;
int start = 7;
int select = 8;
int state;
int led = 2;
int state1;
int state2;


#define  MOT1_DIR   6       // Motor 1 direction
#define  MOT1_STP   5       // Motor 1 step
#define  MOT2_DIR   4       // Motor 2 direction
#define  MOT2_STP   3       // Motor 2 step
#define  M_RIGHT    HIGH    // set cutter motor direction
#define  M_LEFT     LOW
int FEED_STEPS=0; // Feed motor is configured for half-step
#define  CUT_STEPS      19000 // 12-tooth pulley 
 
   

   
//---------------------------- setup() ----------------------------------------
   
void setup() {
  Wire.begin();
  Wire.setClock(400000L);
  //Serial.begin(9600);
#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0

  oled.setFont(Adafruit5x7);
  oled.clear();
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(left, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);
  pinMode(start, INPUT_PULLUP);
  pinMode(select, INPUT_PULLUP);
  pinMode(led, OUTPUT);

  pinMode( MOT1_DIR, OUTPUT );
  pinMode( MOT1_STP, OUTPUT );

  pinMode( MOT2_DIR, OUTPUT );
  pinMode( MOT2_STP, OUTPUT );
  int state = 0;
  int state1 = 0;
  int state2 = 0;
  FEED_STEPS=0; 
}

//---------------------------- loop() -----------------------------------------
//--- Wait for a button press and then call matching function
  
void loop() {
 
   staticmenu();
   
   
   
   
   
   
  //-----------------------------------MAIN MENU-------------------------------------------------------------------


  if (!digitalRead(up) && state == 1) {
    mainmenu();
    cursorup();
    state = 0;
  }

  if (!digitalRead(down) && state == 0 ) {
    mainmenu();
    cursordown();
    state = 1;
  }

  //-----------------------------------MANUAL MENU-------------------------------------------------------------------

  if (state == 1 && !digitalRead(select)) {
    oled.clear();
    staticmenu();
    manualmenu();
    state = 8;
  }

  if (state == 8 && !digitalRead(left)) {
    //staticmenu();
   
    digitalWrite(MOT1_DIR,HIGH);
    for(int x = 0; x < 200; x++) {
      digitalWrite(MOT1_STP,HIGH);
      delayMicroseconds(500);
      digitalWrite(MOT1_STP,LOW);
      delayMicroseconds(500);

  }
  }

  if (state == 8 && !digitalRead(right)) {
    //staticmenu();
    digitalWrite(MOT1_DIR,LOW);
    for(int x = 0; x < 200; x++) {
      digitalWrite(MOT1_STP,HIGH);
      delayMicroseconds(500);
      digitalWrite(MOT1_STP,LOW);
      delayMicroseconds(500);

  }
    
  }
  
   if (state == 8 && !digitalRead(up)) {
    //staticmenu();
    digitalWrite(MOT2_DIR,LOW);
    for(int x = 0; x < CUT_STEPS; x++) {
      digitalWrite(MOT2_STP,HIGH);
      delayMicroseconds(30);
      digitalWrite(MOT2_STP,LOW);
      delayMicroseconds(30);

  }
    
  }
  
  if (state == 8 && !digitalRead(down)) {
    //staticmenu();
    digitalWrite(MOT2_DIR,HIGH);
    for(int x = 0; x < CUT_STEPS; x++) {
      digitalWrite(MOT2_STP,HIGH);
      delayMicroseconds(30);
      digitalWrite(MOT2_STP,LOW);
      delayMicroseconds(30);

  }
    
  }

  if (state == 8 && !digitalRead(start)) {
    oled.clear();
    state = 1;



  }

  //-----------------------------------AUTO MENU QTY-------------------------------------------------------------------


  if (state == 0 && !digitalRead(select)) {
    delay(500);
    oled.clear();
    staticmenu();
    qtymenu();
    state = 4;

  }
  if (!digitalRead(right) && state == 4) {
    state1 += 1;
    delay(200);
    staticmenu();
    qtymenu();
    oled.set1X();
    oled.println();
    oled.set2X();
    oled.setCursor(30, 45);
    oled.print(state1, DEC);
    oled.setCursor(55, 45);
    oled.print("NO.");
  }

  if (!digitalRead(left) && state == 4) {
    state1 -= 1;
    delay(200);
    oled.clear();
    staticmenu();
    qtymenu();
    oled.set1X();
    oled.println();
    oled.set2X();
    oled.setCursor(30, 45);
    oled.print(state1, DEC);
    oled.setCursor(55, 45);
    oled.print("NO.");

  }


  //-----------------------------------AUTO MAIN MENU LENGTH-------------------------------------------------------------------


  if (state == 4 && !digitalRead(select)) {
    delay(500);
    oled.clear();
    staticmenu();
    lengthmenu();
    state = 5;

  }


  if (!digitalRead(right) && state == 5) {
    state2 += 1;
    delay(200);
    staticmenu();
    lengthmenu();
    oled.set1X();
    oled.println();
    oled.set2X();
    oled.setCursor(35, 45);
    oled.print(state2, DEC);
    oled.setCursor(65, 45);
    oled.print("CM");
  }

  if (!digitalRead(left) && state == 5) {
    state2 -= 1;
    delay(200);
    oled.clear();
    staticmenu();
    lengthmenu();
    oled.set1X();
    oled.println();
    oled.set2X();
    oled.setCursor(35, 45);
    oled.println(state2, DEC);
    oled.setCursor(65, 45);
    oled.print("CM");
  }


  //-----------------------------------LAST SCREEN-------------------------------------------------------------------


  if (state == 5 && !digitalRead(select)) {
    delay(500);
    oled.clear();
    staticmenu();
    oled.set2X();
    oled.setCursor(0, 25);
    oled.print("QTY   =");
    oled.setCursor(90, 25);
    oled.println(state1);
    oled.set1X();
    oled.println();
    oled.set2X();
    oled.setCursor(0, 65);
    oled.print("LENGHT=");
    oled.setCursor(90, 65);
    oled.println(state2);
    state = 6;

  }



  //-------------------------------------------------------------------------

 

  if (state == 6 && !digitalRead(start)) {
oled.clear();
    staticmenu();
    oled.set2X();
    oled.setCursor(36, 25);
    oled.print("CUTTING");
    
FEED_STEPS=state2*100;
    
    Run();

oled.clear();
    staticmenu();
    oled.set2X();
    oled.setCursor(35, 25);
    oled.print("FINISH");

state = 12;
  }

if (state == 12 && !digitalRead(start)) {
    oled.clear();
    state = 1;
}
}



void staticmenu() {
  oled.set2X();
  oled.setCursor(0, 0);
  oled.println("CUTTER HMI");
  oled.set1X();
  oled.setCursor(0, 13);
  oled.println("---------------------");
  //oled.display();

}

void mainmenu() {
  oled.set2X();
  oled.setCursor(35, 25);
  oled.println("AUTO  ");
  oled.setCursor(35, 45);
  oled.println("MANUAL");
  //oled.display();

}


void automenu() {
  oled.set2X();
  oled.setCursor(35, 25);
  oled.println("LENGTH");
  oled.setCursor(35, 45);
  oled.println("QTY   ");
  oled.println("             ");
  //oled.display();

}

void cursorup() {
  oled.set2X();
  oled.setCursor(10, 3);
  oled.println(">");
  oled.setCursor(10, 5);
  oled.println(" ");
}

void cursordown() {
  oled.set2X();
  oled.setCursor(10, 3);
  oled.println(" ");
  oled.setCursor(10, 5);
  oled.println(">");
}

void lengthmenu() {
  oled.set2X();
  oled.setCursor(25, 25);
  oled.println("LENGTH");
  //oled.display();
}

void qtymenu() {
  oled.set2X();
  oled.setCursor(35, 25);
  oled.println("QTY   ");

}


void manualmenu() {

  oled.set2X();
  oled.setCursor(0, 65);
  oled.println("MANUAL MODE ON");
  oled.setCursor(50, 200);
  oled.println("ON");

}


//---------------------------- Feed() -----------------------------------------
//--- Feed motor operation
//
void Feed() {
  digitalWrite( MOT1_DIR, HIGH );     // select direction 
  for( int i; i<FEED_STEPS; i++ ) {
    digitalWrite( MOT1_STP, HIGH );
    delayMicroseconds( 10 );          // duration of step signal
    digitalWrite( MOT1_STP, LOW );
    delayMicroseconds( 1000 );        // pause between steps
  }
}

//---------------------------- MoveBlade() ------------------------------------
//--- Move cutting head in desired direction
void MoveBlade( unsigned int mDir ) {
  digitalWrite( MOT2_DIR, mDir );     // select direction 
  for( int k; k<CUT_STEPS; k++ ) {
    digitalWrite( MOT2_STP, HIGH );
    delayMicroseconds( 50 );          // duration of step signal
    digitalWrite( MOT2_STP, LOW );
    delayMicroseconds( 50 );        // pause between steps
  }
}  

//---------------------------- OneCut() ---------------------------------------
//--- Perform a single cut - blade moves back and forth to return it home
void OneCut() {
  MoveBlade( M_RIGHT );
  MoveBlade( M_LEFT );
}

//---------------------------- Run() ------------------------------------------
//--- Perform multiple cuts
void Run() {
  for( int j=0; j<state1; j++ ) {
    Feed();
    MoveBlade( M_RIGHT );
    MoveBlade( M_LEFT );
 }
}



void Feedin() {
  digitalWrite( MOT1_DIR, HIGH );     // select direction 
  for( int l; l<FEED_STEPS; l++ ) {
    digitalWrite( MOT1_STP, HIGH );
    delayMicroseconds( 10 );          // duration of step signal
    digitalWrite( MOT1_STP, LOW );
    delayMicroseconds( 1000 );        // pause between steps
  }
}

//-----------------------------------------------------------------------------
//------ end of FoamCutter.ino ------
