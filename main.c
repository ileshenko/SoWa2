/******************************************************************************
 *  Sun Heater controller emiulation 
 * 
 * inputs
 *   P1.4 ADC - Upper sensor
 *   P1.5 ADC - Lower sensor
 *   P1.3 Button - TBD
 * 
 * output
 *   P1.0 - LED1 (red) - pump motor
 *   P1.6 - LED2 (green) - TBD
 * 
 * 
 ******************************************************************************/

#include <msp430g2553.h>
#include "main.h"
#include "thermistor.h"
#include "timer.h"
#include "leds.h"
#include "buttons.h"
#include "uart.h"

#define D_THEMP_ON 8 //Delta for pumping ON
#define D_THEMP_OFF 3 // Delta for pumping OFF

#define IDLE_MIN_TIME  0, 60
#define PUMPING_MIN_TIME  0, 1
#define IDLE_MAX_TIME   24,0
#define PUMPING_MAX_TIME 0,600

heater_sm_t curr_state;

static void sm_set(heater_sm_t state)
{
	switch (state)
	{
		case SM_IDLE_MIN:
			led_off(LED_PUMP);
			timer_set_alarm(IDLE_MIN_TIME);
			break;
			
		case SM_IDLE:
			timer_set_alarm(IDLE_MAX_TIME);
			break;
			
		case SM_PUMPING_MIN:
			led_on(LED_PUMP);
			timer_set_alarm(PUMPING_MIN_TIME);
			break;
			
		case SM_PUMPING:
			timer_set_alarm(PUMPING_MAX_TIME);
			break;
			
		case SM_CLEANING:
			led_on(LED_PUMP);
			timer_set_alarm(PUMPING_MAX_TIME);
			break;
	}
	curr_state = state;
}

static int sm_toggle_flg;

void sm_toggle(void)
{
	sm_toggle_flg = 1;
}

void sm_init(void)
{
	sm_toggle_flg = 0;
	sm_set(SM_IDLE_MIN);
}

void sm_loop(void)
{
	static unsigned long old_jiffies;
	
	switch (curr_state)
	{
	case SM_IDLE_MIN:
		if (alarm())
			sm_set(SM_IDLE);
		break;

	case SM_IDLE:
		if (themp_delta_get() >= D_THEMP_ON)
			sm_set(SM_PUMPING_MIN);
		else if (alarm())
			sm_set(SM_CLEANING);
		break;
		
	case SM_PUMPING_MIN:
		if (alarm())
			sm_set(SM_PUMPING);
		break;
		
	case SM_PUMPING:
		if ((themp_delta_get() <= D_THEMP_OFF) || alarm())
			sm_set(SM_IDLE_MIN);
		break;
		
	case SM_CLEANING:
		if (alarm())
			sm_set(SM_IDLE_MIN);
	}
	
	if (sm_toggle_flg)
	{
		sm_toggle_flg = 0;
		switch (curr_state)
		{
		case SM_IDLE_MIN:
		case SM_IDLE:
			sm_set(SM_PUMPING_MIN);
			break;
		default:
			sm_set(SM_IDLE_MIN);
			break;
		}
	}
	
	if (old_jiffies != jiffies)
	{
		themps_update();
		uart_report();
		old_jiffies = jiffies;
	}	
}

/* Main Function*/
void main(void)
{ 
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

	timer_init();
    uart_init();
	thermistor_init();
	leds_init();
	buttons_init();
	
	sm_init();
                               
    _EINT();
    for (;;)
    	sm_loop();

}
