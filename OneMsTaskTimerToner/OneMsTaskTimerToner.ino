/*
  timerToner example does 1 task triggered by the OneMsTaskTimer
  -- loop is empty -- all is done in the ISR. mod by AM added toner
  task activated by flash fx when it stops

  The circuit: was opposite but looks like HIGH turns on the built-in LEDs
   LED anodes tied to OUTPUT pins.
   LED cathodes to ground; HIGH turns on lite)

  This example code is in the public domain.

*/ 

#include "OneMsTaskTimer.h"

const int heartbeatPin = RED_LED;  // onboard lite
byte status = 0;  // def as global, not sure it has to be
boolean statusB = 1; // to tone or not to tone
void flash();  // compile fails unless fx declared in advance of use
void toneOnOff();

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

OneMsTaskTimer_t myToner = {1000, toneOnOff, 0, 0};
// after start in flash, fx repeats every 1 sec

void setup()
{
  pinMode(heartbeatPin, OUTPUT);
  OneMsTaskTimer::add(&myTask);
  OneMsTaskTimer::start();
}

void loop()  // nothing to see here folks
{
}

// interrupt handler passed to OneMsTaskTimer obj, called by it periodically
void flash()
{
  if (status % 9 < 7) // mostly ON (high)  // was if (status)
      digitalWrite(heartbeatPin, HIGH);
  else
     digitalWrite(heartbeatPin, LOW);

  // status ^= 0x0001; // bitwise exclusive OR, so inverts value 0/1, 
  // on/off both = period; obscure method but works
  status++;  // I wanted unequal intervals to observe code actions
  if (status > 27)  // and to stop that task to do something else
  { digitalWrite(heartbeatPin, LOW);  // turn lite off and stop task
    // OneMsTaskTimer::stop(); // to stop this task or all?
    OneMsTaskTimer::remove(&myTask); // just suspend this one
    // and start toning
    OneMsTaskTimer::add(&myToner); // repeats with period as initialized
    // OneMsTaskTimer::start();  ?need, N
  }
}  // end flash

void toneOnOff()
{
  tone(8, 2700);  // pin, freq
  delay(800);  // long tone, short off
  noTone(8);
  //  if (statusB) tone(8, 2700);   // don't need pin config for toner
  //  else noTone(8);
  //  statusB = !statusB; // inverts value on alternate calls, on/off each = period
}
