#include <msp430.h>
//#include <stdio.h>
//#include <math.h>
#include "micromouse.h"


int main() {
  while(/*encoder0 != 100*/1) {
    motorInit();
    torque();
  }
  return 0;
}

//interrupts for motor pwm
#pragma vector = TIMER0_A0_VECTOR
__interrupt void ccr0_clear_pulse (void) {
  P1OUT &= ~(BIT2 + BIT1);      // turn off p1.1 and p1.2
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void ccr1_2_set_pulse (void) {
  switch(TAIV) {
  case 2: P1OUT |= BIT1;        // turn on p1.1 (left motor)
    break;
  case 4: P1OUT |= BIT2;        // turn on p1.2 (right motor)
    break;
  }
}

//interrupt for the encoder
#pragma vector = PORT1_VECTOR
__interrupt void falling_edge_capture() {  // captures falling edge from encoder wave
  if(P1IFG & BIT0) {    // if button interrupt is on BIT0,
    encoder0++;        // increment encoder0 count
    P1IFG &= ~BIT0;    // clear interrupt flag for BIT0
  } else if(P1IFG & BIT5) {
    encoder5++;
    P1IFG &= ~BIT5;
  } else if(P1IFG & BIT6) {
    encoder6++;
    P1IFG &= ~BIT6;
  } else if(P1IFG & BIT7) {
    encoder7++;
    P1IFG &= ~BIT7;
  }
}

//interrupt for turning on LEDs
#pragma vector = TIMER1_A1_VECTOR
__interrupt void ta1cc0_isr (void) {
  if(P2OUT == 0x40) {
    P2OUT = 0x01;
  }
  else {
    P2OUT = P2OUT << 1;
  }
}

// ADC10 interrupt service routine
#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR(void) {
  sensorVal[conv_int(P2OUT)] = ADC10MEM;
  ADC10CTL0 &= ~ADC10IFG;
  ADC10CTL0 |= ENC + ADC10SC;
}
