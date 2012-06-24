#ifndef _MAIN_H_
#define _MAIN_H_

typedef enum {
	SM_IDLE_MIN = 0,
	SM_IDLE = 1,
	SM_PUMPING_MIN = 2,
	SM_PUMPING = 3,
	SM_CLEANING = 4
} heater_sm_t;

extern heater_sm_t curr_state;

#endif /*_MAIN_H_*/
