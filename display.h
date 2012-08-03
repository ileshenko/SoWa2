#ifndef _DISPLAY_H_
#define _DISPLAY_H_

void display_init(void);
void display_pulse(void);

/* Negative value means decimal point,
 * 100 - means "--" */ 
void display_set(char val);

typedef enum {
	DISP_NONE,
	DISP_DOWN,
	DISP_UP,
	DISP_CTL,
	DISP_LAST
} display_stat_t;

void display_stat(display_stat_t stat);

void display_test(void);


#endif /*_DISPLAY_H_*/
