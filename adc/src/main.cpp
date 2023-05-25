#include <Arduino.h>
//Setting up PORTF, DDRF, PINF
volatile uint8_t* ptrPORTF = &PORTF;
volatile uint8_t* ptrDDRF = &DDRF;
volatile uint8_t* ptrPINF = &PINF;

// Setting up PORTE, DDRE, PINE
volatile uint8_t* ptrPORTE = &PORTE;
volatile uint8_t* ptrDDRE = &DDRE;
volatile uint8_t* ptrPINE = &PINE;

// Setting up PORTD, DDRD, PIND
volatile uint8_t* ptrPORTD = &PORTD;
volatile uint8_t* ptrDDRD = &DDRD;
volatile uint8_t* ptrPIND = &PIND;

// Setting up PORTC, DDRC, PINC
volatile uint8_t* ptrPORTC = &PORTC;
volatile uint8_t* ptrDDRC = &DDRC;
volatile uint8_t* ptrPINC = &PINC;

// Setting up PORTB, DDRB, PINB
volatile uint8_t* ptrPORTB = &PORTB;
volatile uint8_t* ptrDDRB = &DDRB;
volatile uint8_t* ptrPINB = &PINB;


// when bit 4 is written to one, all pull-ups are disabled even if DDRx and PORTx are configured to pull-up enabled
volatile uint8_t* ptrMCUCR = &MCUCR;

void pinMode(uint8_t port, uint8_t pin, uint8_t value) {
  if (value == 1) {
    if (port == 2) { // Setting port B 66/98
      *ptrDDRB |= (1<<pin);
    } else if (port == 3) { // Setting port C 67/99
      *ptrDDRC |= (1<<pin);
    } else if (port == 4) { // Setting port D 68/100
      *ptrDDRD |= (1<<pin);
    } else if (port == 5) { // Setting port E 69/101
      *ptrDDRE |= (1<<pin);
    } else { // Setting port F
      *ptrDDRF |= (1<<pin);
    }
  } else {
    if (port == 2) { // Setting port B
      *ptrDDRB &= ~(1<<pin);
    } else if (port == 3) { // Setting port C
      *ptrDDRC &= ~(1<<pin);
    } else if (port == 4) { // Setting port D
      *ptrDDRC &= ~(1<<pin);
    } else if (port == 5) { // Setting port E
      *ptrDDRE &= ~(1<<pin);
    } else { // Setting port F
      *ptrDDRF &= ~(1<<pin);
    }
  }

}

void digitalWrite(uint8_t port, uint8_t pin, uint8_t value) {
  if (value == 1) { // if value HIGH, set HIGH
    if (port == 2) {// Setting port B
      *ptrPORTB |= (1<<pin);
    } else if (port == 3) { // Setting port C
      *ptrPORTC |= (1<<pin);
    } else if (port == 4) { // Setting port D
      *ptrPORTD |= (1<<pin);
    } else if (port == 5) { // Setting port E
      *ptrPORTE |= (1<<pin);
    } else { // Setting port F
      *ptrPORTF |= (1<<pin);
    }
  } else {
    if (port == 2) { // Setting port B
      *ptrPORTB &= ~(1<<pin);
    } else if (port == 3) { // Setting port C
      *ptrPORTC &= ~(1<<pin);
    } else if (port == 4) { // Setting port D
      *ptrPORTD &= ~(1<<pin); 
    } else if (port == 5) { // Setting port E
      *ptrPORTE &= ~(1<<pin);
    } else { // Setting port F
      *ptrPORTF &= ~(1<<pin);
    }
  }
}

int digitalRead(uint8_t port, uint8_t pin) {
  if (port == 2) { // Reading port B
    return (*ptrPINB) & (1<<pin);
  } else if (port == 3) { // Reading port C
    return (*ptrPINC) & (1<<pin);
  } else if (port == 4) { // Reading port D
    return (*ptrPIND) & (1<<pin);
  } else if (port == 5) { // Reading port E
    return (*ptrPINE) & (1<<pin);
  } else { // Reading port F
    return (*ptrPINF) & (1<<pin);
  }
}

void setup() {
  Serial.begin(9600);

  ///////////////////////////// Setting up Timer//////////////////////////////////
  TCCR4A = 0b10000010;
  //         **       - channel A compare output mode: Fast PWM, cleared on compare match
  //           **     - channel B compare output mode: normal port operation
  //             **   - force output compare for channel A and B: disabled
  //               *  - pulse width modulator A: enabled
  //                * - pulse width modulator B: disabled
  TCCR4B = 0b00001011;
  //         *        - inversion mode: disabled
  //          *       - prescaler reset: disabled
  //           **     - dead time prescaler: no divsion
  //             **** - prescaler: ck/1024
  TCCR4C = 0b10000000;
  //         **       - shadow bits of COM4A1 and COM4A0
  //           **     - shadow bits of COM4B1 and COM4B0
  //             **   - channel D compare output mode: normal port operation
  //               *  - force output compare for channel D: disabled
  //                * - pulse width modulator D: disabled
  TCCR4D = 0b00000000;
  //         *        - fault protection interrupt: disabled
  //          *       - fault protection mdoe: disabled
  //           *      - fault protection noise canceler: deactivated
  //            *     - fault protection edge: falling edge as trigger
  //             *    - fault protection analog comparator: disabled
  //              *   - fault protection interrupt flag: disabled
  //               ** - wave generation mode: fast pwm
  TCCR4E = 0b00000000;
  //         *        - register update lock: not set
  //          *       - enhanced compare/PWM mode: disabled
  //           ****** - output compare override enable: disabled
  OCR4A = 255;

  ///////////////////////////// Setting up ADC//////////////////////////////////
  ADMUX = 0b01000000;
  //        **        - setting VCC as voltage reference
  //          *       - left adjust result: disabled
  //           *****  - analog channel selection: ADC0
  ADCSRA = 0b10100100;
  //         *        - ADC enable: enabled
  //          *       - ADC conversion: not yet, dont start
  //           *      - auto triggering: enabled
  //            *     - ADC interrupt flag: disabled
  //             *    - ADC interrupt enable: disabled
  //              *** - ADC prescaler: /16
  
  DDRC |= (1<<7); // Port C pin 7 as output, D13
  ADCSRA |= (1<<6); // start conversion

}

void loop() {
  OCR4A = ADCW/1000.0*255;
  Serial.print("Duty cycle: ");
  Serial.print(ADCW/1000.0*100);
  Serial.println("%");
  delay(10);
}


  


 