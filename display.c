#include <msp430g2553.h>
#include "thermistor.h"
#include "display.h"

/*
 *   a
 * f   b
 *   g
 * e   c
 *   d    p
 * */

#define PIN(n) (1<<n)

/* port 1 */

#define D_D PIN(6) 
#define D_G PIN(7) 

#define DP1_MSK ( D_G | D_D )

/* port 2 */
#define D_A PIN(2) 
#define D_B PIN(1)
#define D_C PIN(3)
#define D_E PIN(5) 
#define D_F PIN(0) 

#define D_P PIN(4) 

#define LFT PIN(7) 
#define RGH PIN(6) 

#define DP2_MSK (D_A | D_B | D_C| D_E | D_F | D_P | LFT | RGH ) 

const struct {
	unsigned char p1, p2;
} digimap[] = {
	{ D_D      , D_A | D_B | D_C | D_E | D_F }, // 0
	{         0, D_B       | D_C             }, // 1
	{ D_D | D_G, D_A | D_B       | D_E       }, // 2
	{ D_D | D_G, D_A | D_B | D_C             }, // 3
	{       D_G,       D_B | D_C       | D_F }, // 4
	{ D_D | D_G, D_A       | D_C       | D_F }, // 5
	{ D_D | D_G, D_A       | D_C | D_E | D_F }, // 6
	{         0, D_A | D_B | D_C             }, // 7
	{ D_D | D_G, D_A | D_B | D_C | D_E | D_F }, // 8
	{ D_D | D_G, D_A | D_B | D_C       | D_F }, // 9
	{       D_G,                           0 }, // -
	{         0,                           0 }, // " "
};

#define sleep for (i=0; i<0x8fff; i++);

static void show_fan(void)
{
	volatile unsigned int i;
	P1OUT &= ~DP1_MSK;
	P2OUT &= ~DP2_MSK;

	P2OUT = D_A + RGH + LFT;
	sleep;
	P2OUT = D_B + RGH + LFT;
	sleep;
	P2OUT = D_C + RGH + LFT;
	sleep;
	P1OUT |= D_D;
	sleep;
	P1OUT &= ~D_D;
	P2OUT = D_E + RGH + LFT;
	sleep;
	P2OUT = D_F + RGH + LFT;
	sleep;
	P1OUT |= D_G;
	sleep;
	P2OUT = D_P + RGH + LFT;
	sleep;
}

void display_init(void)
{
	P1DIR |= PIN(6)+PIN(7);
	
	P2DIR |= PIN(0)+PIN(1)+PIN(2)+PIN(3)+PIN(4)+PIN(5)+PIN(6)+PIN(7);
	P2OUT = 0x00;
	
	P2SEL &= ~(PIN(7) + PIN(6));
	P2SEL2 &= ~(PIN(7) + PIN(6));

	show_fan();
}

static char dd[2] = {0,0};
static char point;

inline static void show_digit(char digit, char idx)
{
	P1OUT &= ~DP1_MSK;
	P2OUT &= ~DP2_MSK;
	
	P1OUT |= digimap[digit].p1;
	P2OUT |= digimap[digit].p2 | (idx ? LFT : (RGH | (point ? D_P : 0))) ;
//	P2OUT |= D_A;
//	P2OUT |= RGH;
}

void display_pulse(void)
{
	static char idx = 0;

	show_digit(dd[idx], idx);
	idx = !idx;
}

void display_set(char val)
{
	point = val & 0x80;
	val &= 0x7f;
	
	if (val > 99)
		dd[0] = dd[1] = 10;
	else
	{
		dd[0] = val%10;
		dd[1] = val /10;
	}
}

void display_stat(display_stat_t stat)
{
	switch (stat)
	{
	case DISP_NONE:
		dd[0] = dd[1] = 10;
		break;
	case DISP_DOWN:
		display_set(0x80 | temp_bottom);
		break;
	case DISP_UP:
		display_set(temp_up);
		break;
	case DISP_CTL:
		display_set(temp_ctl);
		break;
	default:
			dd[0] = dd[1] = 11;
	}
}


void display_test(void)
{
//	static int qq = 0;
//	qq++;
//	if (qq >= 10)
//		qq = 0;
//	P2OUT ^= D_E;
//	P1OUT ^= D_C;
}
