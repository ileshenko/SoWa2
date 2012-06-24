#ifndef _LEDS_H_
#define _LEDS_H_

typedef enum {
//	LED_IND,
	LED_PUMP
}leds_t;

void leds_init(void);
void led_on(leds_t led);
void led_off(leds_t led);
void led_toggle(leds_t led);

#endif /*_LEDS_H_*/
