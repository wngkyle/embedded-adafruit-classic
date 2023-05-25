#include <Arduino.h>
#define duty_cycle 50

void setup() {
  // put your setup code here, to run once:
  TCCR1A = 0b00100011; 
  //         **       - channel A normal port operation, disconnected
  //           **     - channel B on compare match, set OCR1A as top
  //             **   - channel C normal port operation, disconnected
  //               ** - waveform generation mode pin 1 and 2 (Fast PWM, OCR1A as top)
  TCCR1B = 0b00011010;
  //         *        - input capture noise canceler deactivated
  //          *       - input capture edge select (falling edge as trigger)
  //           *      - not usedwd
  //            **    - waveform generation mode pin 2 and 3
  //              *** - clock select (clk/8 = 1MHz)
  TCCR1C = 0b00000000;
  //         ***      - force output compare

  OCR1A = 1000;// Will be use as top, at 125th tick
  // OCR1B = 50; // 50% duty cycle
  OCR1B = duty_cycle / 100.0*1000.0; // User-defined duty cycle
  DDRB |= (1<<6);
}

int count = 0;
bool deter = true;

void loop() {
  // Count up and then count down:
  // if (count == 1000) {
  //   deter = false;
  // } else if (count == 0) {
  //   deter = true;
  // }
  // if (deter) {
  //   count++;
  // } else {
  //   count--;
  // }
  // OCR1B = count;
  

  // Counting up to user-defined duty cycle and then count back down:
  // float duty = (duty_cycle / 100.0)*1000;
  // if (count == duty) {
  //   deter = false;
  // } else if (count == 0) {
  //   deter = true;
  // }
  // if (deter) {
  //   count++;
  // } else {
  //   count--;
  // }
  // OCR1B = count;
  
  delay(50);
}