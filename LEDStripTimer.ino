/*
  Base of project is LiquidCrystal Library - Hello World

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

//states in following program
/*
1 - Menu, where you select time to count
2 - Timer, where program counts down
more can be included later for more uses to the LED Bar
*/

// include the library code:
#include <LiquidCrystal.h>

//defines
#define MAX_TIME_MINS 10
#define MAX_TIME MAX_TIME_MINS*60/15  //get max time in 15 second intervals

//function prototypes
void setScreen(int state);
void updateTimer(int* prevTime);
void showInputTime();
void showCountdown(int secs);
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
uint16_t timeInput = 0;      // timer var to hold the time (only need positive)
int totalSeconds;
uint16_t stateCount;
unsigned long timeStart = 0;


LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);

  stateCount = 0;

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  pinMode(pPotentiometer, INPUT);
  pinMode(pSelect, INPUT);

  setScreen(stateCount);
}

//timer application
void loop() {
  //poll the button to see that it works
  bSelect = digitalRead(pSelect);

  //need to change state and debounce the button input (from arduino example)
  if(bSelect != bPrevSelect){
    // only toggle the state if the new button state is HIGH
    if (bSelect == HIGH) {
      stateCount++;
      timeStart = millis();
      //if it goes over, reset state to 0
      if(stateCount >= 2){
        stateCount = 0;
      }
      setScreen(stateCount);
    }
    //delay a bit to avoid bouncing
    delay(50);
  }
  bPrevSelect = bSelect;              //change prevSelect to new value
    
  //if in state 0(menu), repeatitly get the current state of the pot and output it to the timer
  if(stateCount == 0){
    potVal = analogRead(pPotentiometer)/5 * 5;
    timeInput = map(potVal, 0, 1023, 0, MAX_TIME);
    totalSeconds = timeInput*15;
    showInputTime();
  } else if(stateCount == 1){
    updateTimer(&timeStart);
    if(totalSeconds < 0){            //check to see if timer has finished
      timerFinish();
    }
    showCountdown(totalSeconds);
  }
}

//
//FUNCTIONS
//

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

void updateTimer(unsigned long* prevTime){
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
  stateCount = 0;
  setScreen(stateCount);
}