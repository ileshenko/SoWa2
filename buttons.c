
#include "buttons.h"
#include  "msp430g2553.h"

#define BTN_TOGGLE (1<<3)


void buttons_init(void)
{
	P1OUT =  BTN_TOGGLE;
	P1REN |= BTN_TOGGLE; 							// Poll UP resistor
	P1IE |= BTN_TOGGLE;                             // interrupt enabled
	P1IES |= BTN_TOGGLE;                            // Hi/lo edge
	P1IFG &= ~BTN_TOGGLE;                           // IFG cleared
}

void sm_toggle(void);

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
	sm_toggle();
//	P1OUT ^= 0x01;
	P1IFG &= ~BTN_TOGGLE;
}
