#include <Arduino.h>
#include <Keyboard.h>
#include <Adafruit_NeoPixel.h>
#include "RTClib.h"

#define buttonPlayPin 2
#define buttonPreviousPin 4
#define buttonNextPin 6
#define buttonWindowsPin 8
#define ringPin 7
#define ledCount 24

char ctrlKey = KEY_LEFT_CTRL;
char altKey = KEY_LEFT_ALT;
char upKey = KEY_UP_ARROW;
char leftKey = KEY_LEFT_ARROW;
char rightKey = KEY_RIGHT_ARROW;
char windowsKey = KEY_LEFT_GUI;

RTC_DS3231 rtc;
Adafruit_NeoPixel strip(ledCount, ringPin, NEO_GRB + NEO_KHZ800);

DateTime now;
int actualHour;
int actualMinute;
int appointementHour;
int appointementMinute;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(buttonPlayPin, INPUT_PULLUP);
  pinMode(buttonPreviousPin, INPUT_PULLUP);
  pinMode(buttonNextPin, INPUT_PULLUP);
  pinMode(buttonWindowsPin, INPUT_PULLUP);
  Keyboard.begin();

  if (!rtc.begin())
  {
      Serial.println("Couldn't find rtc");
      while(1);
  }

  rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));

  strip.begin();
  strip.show();
  strip.setBrightness(50);
}

void loop() {  
  now = rtc.now();

  actualHour = now.hour();
  actualMinute = now.minute();
  appointementHour = 20;
  appointementMinute = 03;

  if(actualHour == appointementHour && actualMinute == appointementMinute){
    rainbow(10);
  }

  colorWipe(strip.Color(0,0,0),50);
  
  int buttonPlayVal = digitalRead(buttonPlayPin);
  int buttonPreviousVal = digitalRead(buttonPreviousPin);
  int buttonNextVal = digitalRead(buttonNextPin);
  int buttonWindowsVal = digitalRead(buttonWindowsPin);
  //rainbow(10);
  if(buttonPlayVal == 0){
    Keyboard.press(ctrlKey);
    Keyboard.press(altKey);
    Keyboard.press(upKey);
    delay(100);
    Keyboard.releaseAll();
    Serial.println("COUCOU");
  }
  if(buttonPreviousVal == 0){
    Keyboard.press(ctrlKey);
    Keyboard.press(altKey);
    Keyboard.press(leftKey);
    delay(100);
    Keyboard.releaseAll();
  }
  if(buttonNextVal == 0){
    Keyboard.press(ctrlKey);
    Keyboard.press(altKey);
    Keyboard.press(rightKey);
    delay(100);
    Keyboard.releaseAll();
  }
  if(buttonWindowsVal == 0){
    Keyboard.press(windowsKey);
    Keyboard.press('l');
    delay(100);
    Keyboard.releaseAll();
  }
  delay(200);
}

void rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { 
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();
    delay(wait);
  }
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
