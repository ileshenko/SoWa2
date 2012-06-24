
#include "timer.h"
//#include "leds.h"
#include "display.h"

#include <msp430g2553.h>

typedef enum {
	TIMER_OFF,
	TIMER_RUN,
	TIMER_FINISH
} timer_stat_t;

typedef struct {
	unsigned char stat;
	unsigned char hours;
	unsigned int secs;
} timer_t;

timer_t timer;
unsigned long jiffies;

void timer_init(void)
{
	BCSCTL1 = CALBC1_1MHZ;                    // Set DCO to 1MHz
	DCOCTL = CALDCO_1MHZ; 
	BCSCTL2 |= DIVS_0;                        // SMCLK = 1MHz

    TACTL = TASSEL_2 + ID_3 + MC_2;           // SMCLK, 125KHz, contmode

    CCTL0 = CCIE;                             // CCR0 interrupt enabled
    CCR0 += 100;
    
    jiffies = 0;
}

int alarm(void)
{
	return timer.stat == TIMER_FINISH;
}

void timer_set_alarm(char hours, int secs )
{
	_DINT();
	timer.stat = TIMER_RUN;
	timer.hours = hours;
	timer.secs = secs;
	_EINT();
}
  
static unsigned char divider = 0;
// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void main_timer(void)
{
	CCR0 += 15625; // call me 1/8 sec later
	divider++;
	if (divider & 0x7)
		return;
		
	jiffies++;
//	led_toggle(LED_IND);
	display_test();

	if (timer.stat != TIMER_RUN)
		return;
	timer.secs--;
	if (!timer.secs && timer.hours)
	{
		timer.hours--;
		timer.secs = 3600;
	}
	if (!timer.secs)
		timer.stat = TIMER_FINISH;
}
