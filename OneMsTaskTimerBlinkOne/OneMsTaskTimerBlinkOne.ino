/*
  Sample program does one task triggered by the OneMsTaskTimer
  -- loop is empty --- all is done in the ISR.

  The circuit plan was opposite but actually HIGH turns on the built-in LEDs
   LED anodes tie to OUTPUT pins, HIGH turns on lite
   LED cathodes tie to ground; 

  This example code is in the public domain.
*/

#include "OneMsTaskTimer.h"  // libs in __.app/contents/java/libraries

const int heartbeatPin = RED_LED;  // onboard lite
byte status = 0;  // def as global, not sure it has to be
void flash();  // compile fails unless fx declared in advance of use


/* object
    OneMsTaskTimer_t myTask1 ={500, flash1, 0, 0};
   uses following parameters:
   Parameter 1: time in milli seconds between each task call
   Parameter 2: pointer to function called by the task (name of fx)
   Parameter 3: init 0 - only used internally  (so why have it at all?)
   Parameter 4: init 0 - only used internally
   following task calls the flash fx every 1 sec; it alternately turns lite on and off
*/

OneMsTaskTimer_t myTask = {500, flash, 0, 0};

void setup()
{
  pinMode(heartbeatPin, OUTPUT);
  OneMsTaskTimer::add(&myTask); // with period as initialized
  OneMsTaskTimer::start();
}

void loop()
{
}

// interrupt handler 'passed to OneMsTaskTimer' (vs.'called by object')
void flash()
{
  if (status % 9 < 7) // mostly high state, lite ON
  {
    digitalWrite(heartbeatPin, HIGH);
  } else
  {
    digitalWrite(heartbeatPin, LOW);
  }
  // status ^= 0x0001; // bitwise exclusive OR, so inverts value,
  // obscure method but works
  status++;  // I wanted unequal intervals to observe code actions
  if (status > 28)
  { digitalWrite(heartbeatPin, LOW);  // turn lite off and stop task
    // OneMsTaskTimer::stop(); // to stop this task or all?
    OneMsTaskTimer::remove(&myTask); // just suspends this one? Y
  }  // end if
}  // end flash

