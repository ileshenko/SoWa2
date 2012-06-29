#include <msp430g2553.h>
#include "thermistor.h"

//#include "ntc2233600.c"
//#include "ntc1523950.c"
#include "ntc1523950_gnd_471.c"

#define BSIZE 16
const int *ntc; 

int adc_buff[BSIZE];
#define ADC10CTL_INCH(port) (((unsigned int)port)<<12)

int temp_up, temp_bottom;

void thermistor_init(void)
{
    ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON; // TODO ??? SHT2? IE???
    ADC10AE0 = (1<<UP_PORT) + (1<<BOTTOM_PORT); // ADC ports option select
    ADC10DTC1 = BSIZE;                           // Conversions count
    
    temp_up = temp_bottom = 0;
    
    ntc=ntc1523950;
//    ntc=ntc2233600;
}

static inline unsigned int get_average(void)
{
	unsigned int ret = 0;
	int i;
	
	for (i = 0; i< BSIZE; i++)
		ret += adc_buff[i];
		
	return ret/BSIZE;
}

static inline int thermistor_ntc(unsigned int raw)
{
	int ret;
	
	for (ret = 0; ret <100; ret++)
	{
		if (ntc[ret]>=raw)
			return ret;
	}
	return ret;
}

int themp_get(sensor_t sensor)
{
	char port = (sensor == SEN_UP) ? UP_PORT : BOTTOM_PORT;
	unsigned int raw_val;
	
    ADC10CTL0 &= ~ENC;
    while (ADC10CTL1 & BUSY);               // Wait if ADC10 core is active

    ADC10CTL1 = ADC10CTL_INCH(port) + CONSEQ_2; // Set channel. Repeat single channel	

    ADC10SA = (unsigned int)adc_buff;//0x200;                        // Data buffer start
    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion ready
    while (!(ADC10CTL0 & ADC10IFG));               // Wait if ADC10 core is active
    ADC10CTL0 &= ~ADC10IFG;					// Clear interrupt flag
    
    raw_val = get_average();
		
	return  thermistor_ntc(raw_val);
}

void themps_update(void)
{
	temp_up = themp_get(SEN_UP);
    temp_bottom = themp_get(SEN_BOTTOM);
}

int themp_delta_get(void)
{
    if (!temp_up || !temp_bottom)
    	return 0;
    	
    return temp_up - temp_bottom;
}
