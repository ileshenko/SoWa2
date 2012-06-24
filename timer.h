#ifndef _TIMER_H_
#define _TIMER_H_

void timer_init(void);
int alarm(void);
void timer_set_alarm(char hours, int secs );

extern unsigned long jiffies;

#endif /*_TIMER_H_*/
