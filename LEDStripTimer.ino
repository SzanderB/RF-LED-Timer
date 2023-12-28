/*
  Base of project is LiquidCrystal Library - Hello World

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <LiquidCrystal.h>

//defines
#define MAX_TIME_MINS 10
#define MAX_TIME MAX_TIME_MINS*60/15  //get max time in 15 second intervals

//function prototypes
void setScreen(int state);
void timerFinish();

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

//const for potPin and select button
const int pPotentiometer = A0;
const int pSelect = 8;


//globals
int bSelect;            
int bPrevSelect = LOW;

int potVal;
int timerActive = LOW;         //var for loop, when to run timer side
uint16_t timeInput = 0;      // timer var to hold the time (only need positive)
uint16_t totalSeconds;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
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

  setScreen(timerActive);
}

//timer application
void loop() {
  //poll the button to see that it works
  int reading = digitalRead(pSelect);

  //debouncing the button input (from arduino example)
  if(reading != bPrevSelect){
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) { 
    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:
        
    // if the button state has changed:
    if (reading != pSelect) {
      bSelect = reading;

      // only toggle the LED if the new button state is HIGH
      if (bSelect == HIGH) {
        timerActive = !timerActive;
        setScreen(timerActive);
      }
    }
    
    count++;
    //if a button click is acknowledged, read current time and set mins/seconds appropriately
    timeStart = millis();
    }

  //if in state 1(menu), repeatitly get the current state of the pot and output it to the timer
  if(timerActive == LOW){
    potVal = analogRead(pPotentiometer)/5 * 5;
    timeInput = map(potVal, 0, 1023, 0, MAX_TIME);
    showInputTime();
  } else if(timerActive == HIGH){
    updateTimer(&timeStart);
    showCountdown(totalSeconds);
  }
  //update all vars, timer, and LCD
  bPrevSelect = bSelect;              //change prevSelect
}
//resets screen and sets screen for either state
void setScreen(int state){
  lcd.clear();
  if(state == HIGH){
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
  sprintf(curTime, "%d mins %.2d secs", timeInput/4,(timeInput%4)*15);
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

