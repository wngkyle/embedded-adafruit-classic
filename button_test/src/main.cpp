#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_CircuitPlayground.h>

int buttonState = 0;
int buttonPin = 19;

#define NeoPin 17
#define NeoCount 10

Adafruit_NeoPixel strip(NeoCount, NeoPin, NEO_GRB + NEO_KHZ800);
uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t magenta = strip.Color(255, 0, 255);
uint32_t greenishwhite = strip.Color(0, 64, 0, 64);

void setup() {
  // initialize the LED pin as an output:
  pinMode(13, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  // Neo Pixels setup
  strip.begin();
  strip.setBrightness(64);
  strip.show(); // Initialize all pixels to 'off'

}

void loop() {

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(13, HIGH);
    strip.clear();
    strip.fill(green, 0, 10);
    strip.show();
    CircuitPlayground.playTone(8,784,35);
    delay(35);
    CircuitPlayground.playTone(8,784,35);
    delay(35);
    CircuitPlayground.playTone(8,2000,1000);
    delay(35);
  } else {
    // turn LED off:
    digitalWrite(13, LOW);
    strip.clear();
    strip.setPixelColor(1, red);
    strip.show();
  }
  delay(20);
}