//  adding to git 18/01/19

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
Encoder knob1(9, 8);
Encoder knob2(7, 6);
Encoder knob3(5, 4);
Encoder knob4(3, 2);
#define knob1TouchPin  18
#define knob2TouchPin  17
#define knob3TouchPin  16
#define knob4TouchPin  15
#define knob1SwitchPin  22
#define knob2SwitchPin  21
#define knob3SwitchPin  20
#define knob4SwitchPin  19

#include <TimerOne.h>

volatile int sequencePosition = 0;

#include <Adafruit_NeoPixel.h>
#define PIN 10
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_RGB + NEO_KHZ800);

//#include <Bounce.h>
//Bounce button1 = Bounce(29, 15);
int button2 = 30;
int button3 = 31;
//Bounce button4 = Bounce(32, 15);

volatile boolean displayPatternLength = 0;
volatile boolean displayCycleLength = 0;
volatile boolean displayAccelerationVal = 0;
volatile boolean displayTopSpeedVal = 0;

volatile int patternLength = 800;
volatile int cycleLength = 16;
volatile int accelerationVal = 1;
volatile int topSpeedVal = 8;

boolean patternArray1 [64] = {1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0};
boolean patternArray2 [64] = {0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0};
boolean patternArray3 [64] = {0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0};
boolean patternArray4 [64] = {0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0};
unsigned long displayDelay;
int displayHoldTime = 500;



void setup() {
  strip.begin();
  strip.show();
  Serial.begin (9600);
  knob1.write (patternLength);
  knob2.write (cycleLength);
  knob3.write (accelerationVal);
  knob4.write (topSpeedVal);
  Timer1.initialize(150000);
  Timer1.attachInterrupt(moveSequence);
  pinMode (button2, INPUT_PULLUP);
}

void loop()
{
  getEncoderValues ();
  readTouch ();
  displayStuff ();
  if (digitalRead (button2 == HIGH)) {
    sequencePosition = 0;
    //Serial.println (sequencePosition);
  }

}

/////////////////////////////////////////////////////////////////////////////

void getEncoderValues () {
  if (patternLength != (knob1.read() / 4) % 65) {
    patternLength = abs((knob1.read()) / 4) % 65;
    //Serial.println(patternLength);
  }
  if (cycleLength != (knob2.read() / 4) % 64) {
    cycleLength = (knob2.read() / 4) % 64;
    //Serial.println(cycleLength);
  }
  if (accelerationVal != (knob3.read() / 4) % 16) {
    accelerationVal = (knob3.read() / 4) % 16;
    //Serial.println(accelerationVal);
  }
  if (topSpeedVal != (knob4.read() / 4) % 64) {
    topSpeedVal = (knob4.read() / 4) % 64;
    //Serial.println(topSpeedVal);
  }
}

/////////////////////////////////////////////////////////////////////////////

void readTouch ()
{
  auto dat1 = touchRead (knob1TouchPin);
  if (dat1 > 1500) {
    displayPatternLength = 1;
  }
  else {
    displayPatternLength = 0;
  }

  auto dat2 = touchRead (knob2TouchPin);
  if (dat2 > 1950) {
    displayCycleLength = 1;
  }
  else {
    displayCycleLength = 0;
  }

  auto dat3 = touchRead (knob3TouchPin);
  if (dat3 > 1650) {
    displayAccelerationVal = 1;
  }
  else {
    displayAccelerationVal = 0;
  }

  auto dat4 = touchRead (knob4TouchPin);
  if (dat4 > 1700) {
    displayTopSpeedVal = 1;
  }
  else {
    displayTopSpeedVal = 0;
  }
  //Serial.println (dat1);
}

/////////////////////////////////////////////////////////////////////////////

void displayStuff()
{
  if (displayPatternLength == 1)
  {
    for (auto i = 0; i <= 63; i++) {
      auto n = (patternLength) / 16;
      if (i <= (patternLength % 16)) {
        n = n + 1;
      }
      n = n - 1;
      strip.setPixelColor(i, 0, 0, ((n * 16 + i) / 2) + 2);
    }
    for (auto i = (patternLength + 1); i <= 16; i++) {
      strip.setPixelColor(i, 0, 0, 0);
    }
    if (patternLength < 16) {strip.setPixelColor(patternLength % 16 , 0, 0, 40);}
    if (patternLength > 15 && patternLength < 32) {strip.setPixelColor(patternLength % 16 , 40, 40, 0);}
    if (patternLength > 31 && patternLength < 48) {strip.setPixelColor(patternLength % 16 , 0, 40, 0);}
    if (patternLength > 47 && patternLength < 64) {strip.setPixelColor(patternLength % 16 , 40, 40, 40);}
    displayDelay = millis() + displayHoldTime;
  }
  else if (displayCycleLength == 1)
  {
    for (auto i = 0; i <= 63; i++) {
      auto n = (cycleLength) / 16;
      if (i <= (cycleLength % 16)) {
        n = n + 1;
      }
      n = n - 1;
      strip.setPixelColor(i, ((n * 16 + i) / 2) + 2, ((n * 16 + i) / 2) + 2, 0);
    }
    for (auto i = (cycleLength + 1); i <= 16; i++) {
      strip.setPixelColor(i, 0, 0, 0);
    }
    strip.setPixelColor(cycleLength % 16 , 0, 0, 40);
    displayDelay = millis() + displayHoldTime;
  }
  else if (displayAccelerationVal == 1)
  {
    for (int i = 0; i <= accelerationVal; i++) {
      strip.setPixelColor(i, 0, 25, 0);
    }
    for (int i = (accelerationVal + 1); i <= 16; i++) {
      strip.setPixelColor(i, 0, 0, 0);
    }
    displayDelay = millis() + displayHoldTime;
  }
  else if (displayTopSpeedVal == 1)
  {
    for (auto i = 0; i <= 63; i++) {
      auto n = (topSpeedVal) / 16;
      if (i <= (topSpeedVal % 16)) {
        n = n + 1;
      }
      n = n - 1;
      strip.setPixelColor(i, ((n * 16 + i) / 2) + 2, ((n * 16 + i) / 2) + 2, ((n * 16 + i) / 2) + 2);
    }
    for (auto i = (topSpeedVal + 1); i <= 16; i++) {
      strip.setPixelColor(i, 0, 0, 0);
    }
    strip.setPixelColor(topSpeedVal % 16 , 40, 0, 0);
    displayDelay = millis() + displayHoldTime;
  }




  
  else if (millis() > displayDelay) {
    for (int i = 0; i <= 15; i++) {
      if (sequencePosition < 16) {strip.setPixelColor(i, (patternArray1[i] * 10), 0, 0, 40);}
      if (sequencePosition > 15 && sequencePosition < 32) {strip.setPixelColor(i, (patternArray2[i] * 10), 0, 0, 40);}
      if (sequencePosition > 31 && sequencePosition < 48) {strip.setPixelColor(i, (patternArray3[i] * 10), 0, 0, 40);}
      if (sequencePosition > 47 && sequencePosition < 64) {strip.setPixelColor(i, (patternArray4[i] * 10), 0, 0, 40);}
      
      //strip.setPixelColor(i, (patternArray[i] * 10), 0, 0);
    }
    if (sequencePosition < 16) {strip.setPixelColor(sequencePosition % 16 , 0, 0, 40);}
    if (sequencePosition > 15 && sequencePosition < 32) {strip.setPixelColor(sequencePosition % 16, 40, 40, 0);}
    if (sequencePosition > 31 && sequencePosition < 48) {strip.setPixelColor(sequencePosition % 16, 0, 40, 0);}
    if (sequencePosition > 47 && sequencePosition < 64) {strip.setPixelColor(sequencePosition % 16, 40, 40, 40);}
    
    
 //   strip.setPixelColor(sequencePosition, 15, 25, 0);
  }
  strip.show();
}

void moveSequence () {
  sequencePosition = (sequencePosition + 1) % patternLength;
}
