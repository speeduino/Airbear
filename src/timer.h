#ifndef TIMER_H
#define TIMER_H

void initTimers();
void stopTimers();
void ARDUINO_ISR_ATTR oneMSInterval(void);

#endif