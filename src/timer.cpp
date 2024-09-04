#include "globals.h"
#include "timer.h"

#ifdef ESP8266
  #include "ESP8266TimerInterrupt.h"
  ESP8266Timer ITimer0();
#endif

#ifdef ESP32
  #include "ESP32TimerInterrupt.h"
  ESP32Timer ITimer0(0); 
  ESP32_ISR_Timer ISR_Timer;
#endif

#define TIMER0_INTERVAL_MS 1000

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

  ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS, oneMSInterval);

}

void stopTimers()
{
  ITimer0.detachInterrupt();
}

bool IRAM_ATTR oneMSInterval(void *timerNo)
{
  ISR_Timer.run();
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

  return true;
}