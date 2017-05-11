// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            9

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      24

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//choose a pattern between 0-255
byte pattern = 150; //cool patterns: 90, 150, 182, 201
int delayval = 100; //delay
int level = 40; //brightness
bool lights[] = {true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
bool ranPattern = true; //do you want to cycle through random patterns?
int seedNumber = 2; //seed values (seedNumber > 0)
int bigPrime = 251;
int largePrime = 257;

void tick(int pattern) {
  bool nextLights[] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};

  for (int i = 0; i < NUMPIXELS; i++) {
    nextLights[i] = lightState (i, pattern);
  }
  transferLight(nextLights);
  setLights(level);
  delay(delayval);
}

void transferLight(bool nextLights[]) {
  for (int i = 0; i < NUMPIXELS; i++) {
    lights[i] = nextLights[i];
  }
}

bool lightState (int aLight, int pattern) {
  int number = lightNumber(aLight);
  return bitShift(pattern, number);
}

int lightNumber (int aLight) {
  int on = 0;
  int number = 0;
  for (int i = 0; i < 4; i++){
    on = oneZero(lights[(aLight + 1 - i + NUMPIXELS) % NUMPIXELS]);
    number = number + twoPower(i) * on;
  }
  return number;
}

void setLights(int level) {

  for (int i = 0; i < NUMPIXELS; i++){
    blueLight(i, oneZero(lights[i]) * level);
  }
  
}

bool bitShift(int aNumber, int shiftNumber) {
  for (int i = 0; i < shiftNumber; i++) {
    aNumber = aNumber / 2;
  }
  return oneZero(aNumber % 2);
}

int oneZero(bool aBool) {

  if (aBool) {
    return 1;
  }
  return 0;
  
}

void resetLights() {

  for (int i = 0; i < NUMPIXELS; i++){
    blueLight(i, 0);
  }
  
}

void newArray() {
  bool nextLights[] = {true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
  transferLight(nextLights);
}

void blueLight (int aLight, int level) {

  aLight = (aLight + NUMPIXELS * 10) % NUMPIXELS;
  pixels.setPixelColor(aLight, pixels.Color(0,0,level));
  pixels.show();
  
}

int twoPower(int number) {
  
  int twos = 1;
  for (int i = 0; i < number; i++) {
    twos = twos * 2;
  }
  return twos;
  
}

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {

  if (ranPattern) {
    newArray();
    seedNumber = (seedNumber * bigPrime) % largePrime;
    for (int i = 0; i < NUMPIXELS * 2; i++) {
      tick(seedNumber % 256);
    }
  } else {
    tick(pattern);
  }
  
}
