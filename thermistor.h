#ifndef _THERMISTOR_H_
#define _THERMISTOR_H_

#define UP_PORT 4
#define BOTTOM_PORT 5

extern int temp_up, temp_bottom;

typedef enum {
	SEN_UP,
	SEN_BOTTOM
} sensor_t;

void thermistor_init(void);
int themp_get(sensor_t sensor);
void themps_update(void);
/* Does not actually measure themperatures. Call themps_update() before */
int themp_delta_get(void);

#endif /*_THERMISTOR_H_*/
