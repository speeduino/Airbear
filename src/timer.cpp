#include "globals.h"
#include "timer.h"

#define TIMER0_INTERVAL_MS 1000
hw_timer_t *timer = NULL;

volatile byte loop5ms;
volatile byte loop33ms;
volatile byte loop66ms;
volatile byte loop100ms;
volatile byte loop250ms;
volatile int loopSec;

void initTimers()
{
  loop5ms = 0;
  loop33ms = 0;
  loop66ms = 0;
  loop100ms = 0;
  loop250ms = 0;
  loopSec = 0;

  // Set timer frequency to 1Mhz
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &oneMSInterval, true);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);

}

void stopTimers()
{
  timerDetachInterrupt(timer);
}

void ARDUINO_ISR_ATTR oneMSInterval()
{
  BIT_SET(TIMER_mask, BIT_TIMER_1KHZ);

  //Increment Loop Counters
  loop5ms++;
  loop33ms++;
  loop66ms++;
  loop100ms++;
  loop250ms++;
  loopSec++;

  //200Hz loop
  if (loop5ms == 5)
  {
    loop5ms = 0; //Reset counter
    BIT_SET(TIMER_mask, BIT_TIMER_200HZ);
  }

  //30Hz loop
  if (loop33ms == 33)
  {
    loop33ms = 0; //Reset counter
    BIT_SET(TIMER_mask, BIT_TIMER_30HZ);
  }

  //15Hz loop
  if (loop66ms == 66)
  {
    loop66ms = 0; //Reset counter
    BIT_SET(TIMER_mask, BIT_TIMER_15HZ);
  }

  //10Hz loop
  if (loop100ms == 100)
  {
    loop100ms = 0; //Reset counter
    BIT_SET(TIMER_mask, BIT_TIMER_10HZ);
  }

  //4Hz loop
  if (loop250ms == 250)
  {
    loop250ms = 0; //Reset counter
    BIT_SET(TIMER_mask, BIT_TIMER_4HZ);
  }

  //1Hz loop
  if (loopSec == 1000)
  {
    loopSec = 0; //Reset counter
    BIT_SET(TIMER_mask, BIT_TIMER_1HZ);
  }
}