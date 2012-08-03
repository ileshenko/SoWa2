
#include "uart.h"
#include "timer.h"
#include "thermistor.h"
#include "main.h"
#include <msp430g2553.h>
#include <stdio.h>

char report[128];
unsigned int i;

void uart_init(void)
{
#if 0
	BCSCTL1 = CALBC1_1MHZ; 		    // Set range
  	DCOCTL = CALDCO_1MHZ;  		    // Set DCO step + modulation
#endif
  	
	/* Port 1.2 (Rx) may be overriden by ADC if we need output only */
	P1SEL |= BIT1 + BIT2; //select RXD and TXD for UART
	P1SEL2 |= BIT1 + BIT2; //select RXD and TXD for UART
	
	UCA0CTL0 = 0;
	UCA0CTL1 |= UCSSEL_3;
	
	UCA0MCTL = UCBRF_0 + UCBRS_1;

	UCA0BR0 = 104;
	UCA0BR1 = 0;
	
	UCA0CTL1 &= ~UCSWRST; //toggle swreset off
}

static char *cat_ul(char *buf, unsigned long val)
{
	unsigned long cutoffVal = val/10;
	
	if (cutoffVal)
		buf = cat_ul(buf, cutoffVal);
	
	*buf = '0' + val%10;

	return buf+1;
}

static char *cat_str(char *buf, char *str)
{
	while (*str)
		*buf++ = *str++;
	return buf;
}

void uart_report(void)
{
	char *buf = report;
	
	buf = cat_ul(buf, jiffies);
	buf = cat_str(buf, "\t");
	buf = cat_ul(buf, curr_state);
	buf = cat_str(buf, "\t");
	buf = cat_ul(buf, temp_up);
	buf = cat_str(buf, "\t");
	buf = cat_ul(buf, temp_bottom);
	buf = cat_str(buf, "\t");
	buf = cat_ul(buf, temp_ctl);
	buf = cat_str(buf, "\r\n");
	
	*buf = 0; 
	i = 0;
	IE2 |= UCA0TXIE;
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR (void)
{
  UCA0TXBUF = report[i++];					// TX next character

  if (!report[i])							// TX over?
    IE2 &= ~UCA0TXIE;                       // Disable USCI_A0 TX interrupt
}

