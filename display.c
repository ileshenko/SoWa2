#include <msp430g2553.h>
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


/* port 2 */
#define D_A PIN(2) 
#define D_B PIN(0) 
#define D_D PIN(5) 
#define D_E PIN(4) 
#define D_F PIN(1)

#define D_P PIN(3)
 
//#define LFT (PORT(1) | PIN(8)) 


struct {
	int p1, p2;
} digimap[] = {
//	{0,0}
	{ D_A | D_B | D_C | D_D | D_E | D_F, 0}
};


void display_init(void)
{
	P1DIR |= PIN(6)+PIN(7);
	
	P2DIR |= PIN(0)+PIN(1)+PIN(2)+PIN(3)+PIN(4)+PIN(5)+PIN(6)+PIN(7);
	P2OUT = 0x00;
	
	P2SEL &= ~(PIN(7) + PIN(6));
	P2SEL2 &= ~(PIN(7) + PIN(6));
	
	P2OUT |= PIN(7) + PIN(6);
	
	
}

void display_test(void)
{
//	P2OUT ^= D_E;
	P1OUT ^= D_C;
}
