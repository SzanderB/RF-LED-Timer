/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/





// include the library code:
#include <LiquidCrystal.h>

//defines
#define MAX_TIME 10 * 60 / 15 //max seconds, but split into 15 second intervals


//function prototypes
void setScreen(int state);
void timerFinish();

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

//const for potPin and select button
const int pPotentiometer = A0;
const int pSelect = 8;

int vSelect;
int vPrevSelect;
int vPot;
int active = 1;         //var for loop, when to run timer side
uint16_t time = 0;      // timer var to hold the time (only need positive)
uint16_t totalSeconds;
unsigned long lastDebounceTime = 0;
uint16_t debounceDelay;
uint16_t count;
uint16_t timeStart = 0;


LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);

  count = 0;
  lastDebounceTime = 0;  
  debounceDelay = 50; 

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  pinMode(pPotentiometer, INPUT);
  pinMode(pSelect, INPUT);

  setScreen(active);
}

//timer application
void loop() {
  //poll the button to see that it works
  vSelect = digitalRead(pSelect);

  //debouncing the button input
  if(vSelect != vPrevSelect){
    lastDebounceTime = millis();
    // every time the button state changes, get the time of that change FROM: https://digilent.com/reference/learn/microprocessor/tutorials/debouncing-via-software/start#:~:text=Software%20debouncing%20is%20accomplished%20by,whether%20consecutive%20samples%20are%20received.
    if (((millis() - lastDebounceTime) > debounceDelay) && (lastDebounceTime != 0)) { 
    /*
    *if the difference between the last time the button changed is greater
          *than the delay period, it is safe to say
          *the button is in the final steady state, so set the LED state to
          *button state.
    */
      //if its currently in countdown state, reset it. If not, move to countdown state
    if(active == 2){
      active = 1;
    } else {
      active = 2;
      totalSeconds = time * 15;
    }
    setScreen(active);
    count++;
    //if a button click is acknowledged, read current time and set mins/seconds appropriately
    timeStart = millis();
    }
  }


  //if in state 1(menu), repeatitly get the current state of the pot and output it to the timer
  if(active == 1){
    vPot = analogRead(pPotentiometer)/5 * 5;
    time = map(vPot, 0, 1023, 0, MAX_TIME);
    showInputTime();
  } else if(active == 2){
    updateTimer(&timeStart);
    showCountdown(totalSeconds);
  }
  //update all vars, timer, and LCD
  vPrevSelect = vSelect;              //change prevSelect
}
//resets screen and sets screen for either state
void setScreen(int state){
  lcd.clear();
  if(state == 1){
    lcd.setCursor(0, 0);
    lcd.print("Time Remaining:");
  } else{
    lcd.setCursor(0, 0);
    lcd.print("Enter Time:");
  }
}
//updates the time being input to the screen
void showInputTime(){
  char curTime[15];
  sprintf(curTime, "%d mins %.2d secs", time/4,(time%4)*15);
  lcd.setCursor(0, 1);
  lcd.print(curTime);
}
//
void showCountdown(int secs){
  char timeRemaining[6];

  lcd.setCursor(0, 1);
  sprintf(timeRemaining, "%d:%.2d", secs/60, secs%60);
  lcd.print(timeRemaining);
  
}

void updateTimer(int* prevTime){
  if(millis() - *prevTime >= 1000){
    *prevTime = millis();
    totalSeconds--;
  }
}

//timer finish will pulse red on the LED Strip and reset to menu
void timerFinish(){
  //led strip red

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Timer Complete!");
  delay (5000);     //5 sec delay before reset
}

