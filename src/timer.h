#ifndef TIMER_H
#define TIMER_H

void initTimers();
void stopTimers();
bool IRAM_ATTR oneMSInterval(void *timerNo);

#endif