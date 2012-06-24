#include <msp430g2553.h>
#include "leds.h"

#define LED_R (1<<0)
#define LED_G (1<<6)

static unsigned char map_leds(leds_t led)
{
	return (led == LED_IND)? LED_G : LED_R;
}

void leds_init(void)
{
	P1DIR |= LED_R + LED_G;
	P1OUT = 0x00;
}

void led_on(leds_t led)
{
	P1OUT |= map_leds(led);
}

void led_off(leds_t led)
{
	P1OUT &= ~(map_leds(led));
}

void led_toggle(leds_t led)
{
	P1OUT ^= map_leds(led);
}
