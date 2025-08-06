// include the library code:
#include <FastLED.h>
#include <IRremote.h>

//defines
#define NUM_LEDS 60


#define SECONDS_30  0x8778C7EA
#define SECONDS_45  0x9D62C7EA
#define SECONDS_60  0x9E61C7EA
#define SECONDS_75  0xCB34C7EA
#define SECONDS_90  0xB34CC7EA
#define SECONDS_120 0xAA55C7EA
#define OFF         0xD52AC7EA

#define POWER 0.5

//function prototypes
void showRed();
void displayLEDStrip();
void restartTimer(int seconds);
void stopTimer();

//const for IR Pin
const int LED_DATA_PIN = 3;
const int IR_PIN = 2;


//globals
volatile int ledsRemaining = NUM_LEDS;
volatile uint16_t preloadValue = 0;

unsigned long lastCommandTime = 0;
unsigned long lastDisplay = 0;

bool buzz = false;
unsigned long buzzStartTime = 0;
bool isBuzzing = false;

bool activeTimer = false;

const int buzzerPin = 5;  // Pin connected to transistor base through resistor
const int toneFreq = 200; // Resonant frequency of DBX05LFPN
const int toneDuration = 500; // Duration of tone in milliseconds

CRGB leds[NUM_LEDS];


void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  pinMode(buzzerPin, OUTPUT);
  FastLED.clear();  // clear all pixel data
  FastLED.show();

  // enable IR receiver
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);
  IrReceiver.resume();
  // turn on timer interrupt
  // set up Timer 1
  TCCR1A = 0;          // normal operation
  TCCR1B = 0x00;       // Timer off for now
  // Enable overflow interrupt
  TIFR1 |= (1 << TOV1); // Clear overflow flag
  TIMSK1 = 0x01;
}

//timer application
void loop() {
  //if a new message received
  if (IrReceiver.decode()) {
    if(millis() - lastCommandTime < 500){
      IrReceiver.resume();
      return;
    }

    uint32_t cmd = IrReceiver.decodedIRData.decodedRawData;
    Serial.println(cmd, HEX);
    lastCommandTime = millis();

    // Ignore bad ones
    if (cmd == SECONDS_30 || cmd == SECONDS_45 || cmd == SECONDS_60 || cmd == SECONDS_75 || cmd == SECONDS_90 || cmd == SECONDS_120 || cmd == OFF) {

      stopTimer();
      FastLED.clear();
      FastLED.show();
      ledsRemaining = NUM_LEDS;

      switch (cmd) {
        case SECONDS_30:
          restartTimer(30);
          break;
        case SECONDS_45:
          restartTimer(45);
          break;
        case SECONDS_60:
          restartTimer(60);
          break;
        case SECONDS_75:
          restartTimer(75);
          break;
        case SECONDS_90:
          restartTimer(90);
          break;
        case SECONDS_120:
          restartTimer(120);
          break;
        case OFF:
          ledsRemaining = 0;
          activeTimer = false;
          break;
        default:
          break;
        }
      }
    IrReceiver.resume();
  }

  //only update leds every 250ms because .show() disables interrupts. If it happens too frequently, the IRremote wont be able to function properly
  // IR remote works on interrupts, so if interrupts are turned off every ms, it will never receive the right data
  if((millis() - lastDisplay > 250) && activeTimer){
    displayLEDStrip();
    lastDisplay = millis();
  }
  

  if (buzz && !isBuzzing) {
    tone(buzzerPin, toneFreq);
    buzzStartTime = millis();
    isBuzzing = true;
    buzz = false;
  }

  if (isBuzzing && (millis() - buzzStartTime >= toneDuration)) {
    activeTimer = false;
    noTone(buzzerPin);
    isBuzzing = false;
    // Reset IR receiver to restore its timing since both tone and it use Timer 2
    IrReceiver.stop();         
    IrReceiver.begin(IR_PIN);
    IrReceiver.resume();
  }
}


//
//FUNCTIONS
//

void displayLEDStrip(){
  if(ledsRemaining <= 0){
    FastLED.clear();  // clear all pixel data
    FastLED.show();
  }else{
    fadeToBlackBy(leds, NUM_LEDS, 255);
    int colorLED = map(ledsRemaining, 0, NUM_LEDS, 0, 96);
    for(int i = 0; i < ledsRemaining; i++) 
      {
        leds[i] = CHSV (colorLED,255,128);
      }
    FastLED.show();
  }
}

void stopTimer(){
  //turn on timer with prescaler 1/64
  TCCR1B = 0x00;
}
//This is a little confusing, its not exactly doing 1 overflow equals 1 second. Its 1 overflow takes 1 LED off. So we have 60 LEDs, so if its 120 seconds, then its 1 LED every 2 seconds
void restartTimer(int seconds){ 
  //Math to get what to preload
  //1 MHz clock, 64 prescaler
  //15625 ticks/sec
  // So we need seconds / NUM_LEDS * 15625 ticks for 1 LED
  activeTimer = true;
  
  preloadValue = 65535 - (uint16_t)((float)seconds / NUM_LEDS * 15625);
  // Preload timer to overflow properly
  TCNT1H = (0xFF00 & preloadValue) >> 8;
  TCNT1L = 0x00FF & preloadValue;
  TIFR1 |= (1 << TOV1); // Clear any pending overflow flag
  //turn on timer with prescaler 1/1024
  TCCR1B = 0x05;
}

void showRed(){
  for(int i = 0; i < NUM_LEDS; i++) 
    {
      leds[i] = CRGB::Red;
    }
}
// Interrupts
ISR(TIMER1_OVF_vect){
  if((--ledsRemaining) <= 0){
    stopTimer();
    buzz = true;
  }else{
    // Preload timer to overflow properly
    TCNT1H = (0xFF00 & preloadValue) >> 8;
    TCNT1L = 0x00FF & preloadValue;
  }
}
