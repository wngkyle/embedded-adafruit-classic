#include <Arduino.h>
#include <Wire.h>

byte val[4];
char index = 0;
short IRData;
short LEDData; 

ISR(INT3_vect) { 
  cli(); // clear the bit, disable the interrupt, prevent the interrupt to be called again while it is still in the function
  PORTC |= (1<<7); // turn PC7 (LED) on
  GetData(); // calls the GetData() function below to 
  IRData = (val[0] << 7) + val[1]; // IR values come first in FIFO data register, first 2 bytes are IR values
  LEDData = (val[2] << 7) + val[3]; // red LED values come after IR values in FIFO data register, last 2 bytes are red LED values
  Serial.println("IR Data: " + IRData); // prints out the IR LED values
  Serial.println("LEDData: " + LEDData); // prints  out the Red LED values
  delay(100); // delay for 0.1 second to create LED blinking
  PORTC &= ~(1<<7); // turn PC7 (LED) off to create blinking
  sei(); // set the bit, enable the interrupt
}

void GetData() {
  // Retrieving samples
  Wire.beginTransmission(0b10101111); // slave address + write mode
  Wire.write(0x05); // register address
  Wire.write(0b10101110); // slave address + read mode
  Wire.endTransmission(false); // end transmission 

  Wire.requestFrom(0x05, 4); // request bytes from the slave device
                             // request 4 bytes since one complete sample is 4 bytes

  // Method 1 of reading data
  for (char i = 0; i < 4; i++) { 
    val[i] = Wire.read();
    Serial.println("Sample data: " + val[i]);
  }

  // // Method 2 of reading data
  // while(Wire.available()){ // Reads from the sample 
  //   val[index] = Wire.read();
  //   Serial.println("Sample data: " + val[index]);
  //   index = index + 1;
  // }
  // index = 0; // reset the variable index to prepare for the next read 
}

void setup() {
  // Setting up I2C
  TWCR = 0b01000101;
  //       *        interrupt flag
  //        *       enable acknowledge bit
  //         *      start condition bit
  //          *     stop condition bit
  //           *    write collision flag
  //            *   TWI enable bit
  //             *  reserved bit
  //              * interrupt enable

  Serial.begin(9600); // start the serial bus to allow displaying values

  // Configuring GPIO pin PD3 as input
  EICRA |= (1<<7) | (1<<6); // INT3 rising edge interrupt
  DDRD &= ~(1<<3); // Turn PD3 as input
  PORTD |= (1<<3); // Turn on pull up resistor, so int does not float around and go crazy
                   // When port is configured as input, the pull up resistor is activated
  EIMSK |= (1<<3); // Turn on INT3, arm the interrupt
  sei(); // enable interrupt

  // Setting onboard LED as output
  DDRC |= (1<<7); // set onboard LED as output
  
  Wire.begin(); // join I2C bus
  
  // Configuring MAX30100
  // Configuring interrupt status register 
  Wire.beginTransmission(0b10101111); // slave address + write mode
  Wire.write(0x00); // register address
  Wire.write(0b10101111); // slave address + write mode
  Wire.write(0b00000000); // clear the flags first to ensure they are in a known state
  Wire.endTransmission(false);
  // Configuring interrupt enable register 
  Wire.beginTransmission(0b10101111); // slave address + write mode
  Wire.write(0x01); // register address
  Wire.write(0b10101111); // slave address + write mode
  Wire.write(0b1010000); // turn on FIFO almost full interrupt and SPO2 interrupt
  Wire.endTransmission(false);
  // Configuring sensor mode
  Wire.beginTransmission(0b10101111); // slave address + write mode
  Wire.write(0x06); // register address
  Wire.write(0b10101111); // slave address + write mode
  Wire.write(0b00000011); // enabled SPO2 
  Wire.endTransmission(false);
  // Configuring SPO2 register
  Wire.beginTransmission(0b10101111); // slave address + write mode
  Wire.write(0x07); // register address
  Wire.write(0b10101111); // slave address + write mode
  Wire.write(0b01010011); // enabled high resolution mode, samples rate 400 per second, led pulse width 1600 microseconds, ADC resolution 16 bits
  Wire.endTransmission(false);
}

void loop() {
  // put your main code here, to run repeatedly:
}