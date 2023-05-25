
#include <Arduino.h>

void setup(){
  // Set port pd0, pd1, pd2 as output
  DDRD |= (7<<0);

  //Set F6 as input (start and stop button)
  DDRF &= ~(1<<6);
  //Set D4 as input (reset button)
  DDRD &= ~(1<<4);
}

unsigned int count = 0;
unsigned int clockcount = 0;
unsigned char started = 0;
void loop (){
  //reset the counter if the reset button if pressed or reaching 3bit max
  if(count > 7 | (PIND & (1<<4))){
    count = 0;
  }
  //Only change the counter and LED if the button is pressed
  if(PINF & (1<<6)){
    started ^= (1<<0);
  }
  //introduce clock delays
  if(clockcount == 65000){
    clockcount = 0;
  }
  //only update at the end of the clock cycle
  if(started & clockcount == 64999){
    //Set port according to the count
    if(count & (1<<0)){
      PORTD |= (1<<0);
    }
    else{
      PORTD &= ~(1<<0);
    }
    if(count & (1<<1)){
      PORTD |= (1<<1);
    }
    else{
      PORTD &= ~(1<<1);
    }
    if(count & (1<<2)){
      PORTD |= (1<<2);
    }
    else{
      PORTD &= ~(1<<2);
    }
    //increment the count
    Serial.println(PORTD);
    ++count;
  }
  ++clockcount;
  //delay for a second
  // delay(1000);
}

