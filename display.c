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


#define PORT(n) (n<<8)
#define PIN(n) (1<<n)
/*
#define D_A (PORT(2) | PIN(0)) 
#define D_B (PORT(2) | PIN(0)) 
#define D_C (PORT(2) | PIN(2)) 
#define D_D (PORT(2) | PIN(3)) 
#define D_E (PORT(2) | PIN(4)) 
#define D_F (PORT(2) | PIN(5))
 
#define D_G (PORT(1) | PIN(6)) 
#define D_P (PORT(1) | PIN(7))
*/

/* port 1 */

#define D_C PIN(6) 
#define D_G PIN(7) 

#define DP1_MSK ( D_C | D_G )

/* port 2 */
#define D_A PIN(2) 
#define D_B PIN(0) 
#define D_D PIN(5) 
#define D_E PIN(4) 
#define D_F PIN(1)

#define D_P PIN(3)

#define LFT PIN(7) 
#define RGH PIN(6) 

#define DP2_MSK (D_A | D_B | D_D | D_E | D_F | D_P | LFT | RGH ) 

const struct {
	unsigned char p1, p2;
} digimap[] = {
	{ D_C      , D_A | D_B | D_D | D_E | D_F }, // 0
	{ D_C      , D_B                         }, // 1
	{ D_G      , D_A | D_B | D_D | D_E       }, // 2
	{ D_C | D_G, D_A | D_B | D_D             }, // 3
	{ D_C | D_G,       D_B             | D_F }, // 4
	{ D_C | D_G, D_A       | D_D       | D_F }, // 5
	{ D_C | D_G, D_A       | D_D | D_E | D_F }, // 6
	{ D_C      , D_A | D_B                   }, // 7
	{ D_C | D_G, D_A | D_B | D_D | D_E | D_F }, // 8
	{ D_C | D_G, D_A | D_B | D_D       | D_F }, // 9
	{       D_G,                           0 }, // -
	{         0,                           0 }, // " "
};


void display_init(void)
{
	P1DIR |= PIN(6)+PIN(7);
	
	P2DIR |= PIN(0)+PIN(1)+PIN(2)+PIN(3)+PIN(4)+PIN(5)+PIN(6)+PIN(7);
	P2OUT = 0x00;
	
	P2SEL &= ~(PIN(7) + PIN(6));
	P2SEL2 &= ~(PIN(7) + PIN(6));
	
//	P2OUT |= PIN(7) + PIN(6);
//	P2OUT |= LFT;
	
	
}

static char dd[2] = {8,1};
static char point;

inline static void show_digit(char digit, char idx)
{
	P1OUT &= ~DP1_MSK;
	P2OUT &= ~DP2_MSK;
	
	P1OUT |= digimap[digit].p1;
	P2OUT |= digimap[digit].p2 | (idx ? LFT : (RGH | (point ? D_P : 0))) ;
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
	if (point)
	    val =-val;
	
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
		display_set(-temp_bottom);
		break;
	case DISP_UP:
		display_set(temp_up);
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
