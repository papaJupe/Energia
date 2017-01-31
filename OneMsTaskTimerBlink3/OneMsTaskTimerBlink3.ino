/*
 Sample program does 3 tasks triggered by the OneMsTaskTimer;
 loop is empty as all is done in the ISR running 3 tasks with 
 2 on-board LEDs.

 The circuit:
 * LED common cathodes tied to gnd.
 * LED anodes connected to I/O pins (but here using built-ins)
  
 This example code is in the public domain.
 
*/  

#include "OneMsTaskTimer.h"

const int heartbeatPin1 = RED_LED;
const int heartbeatPin2 = GREEN_LED;
void flash1(), flash2(), flash3();

/* object
    OneMsTaskTimer_t myTask1 ={500, flash1, 0, 0};
   uses following parameters:
   Parameter 1: time in milli seconds between each task call
   Parameter 2: pointer to function called by the task
   Parameter 3: init 0 - only used internally  (so why have it at all?)
   Parameter 4: init 0 - only used internally
 */  
 
OneMsTaskTimer_t myTask1 ={500, flash1, 0, 0}; // call flash1 every .5 sec, turn off red
OneMsTaskTimer_t myTask2 ={1000, flash2, 0, 0};  // flash2 every 1 sec. turn off grn
OneMsTaskTimer_t myTask3 ={2000, flash3, 0, 0};  // flash3 turn on both

void setup()  { 
  // open the hardware serial port if printing
  // Serial.begin(115200);
  pinMode(heartbeatPin1, OUTPUT);
  pinMode(heartbeatPin2, OUTPUT);
  OneMsTaskTimer::add(&myTask1); // 500ms period
  OneMsTaskTimer::add(&myTask2); // 1000ms period
  OneMsTaskTimer::add(&myTask3); // 2000ms period
  OneMsTaskTimer::start();

} 

void loop()  { 
}

// interrupt handler passed to OneMsTaskTimer
void flash1(){  // HIGH turns red lite on; should be every 500msec
  // print every time the handler is called ?
  // Practically not a good idea as any function inside an ISR should be as fast as possilbe
  // Serial.print(":1");
  //heartbeatPinOn1 = !heartbeatPinOn1;  var not defined or used ? invert something
  digitalWrite(heartbeatPin1,HIGH);  // turns lite on
}

// interrupt handler passed to OneMsTaskTimer
void flash2(){  // HIGH turns grn lite on; should be every 1000msec
  // log every time the handler is called 
  // Practically not a good idea as this function inside an ISR should be as short as possilbe
  // Serial.print(":2");
  //heartbeatPinOn2 = !heartbeatPinOn2; var not defined or used
  digitalWrite(heartbeatPin2,HIGH);
}
  
// interrupt handler passed to OneMsTaskTimer
void flash3(){  // LOW turns off both lites; should be every 2000msec
  // log every time the handler is called
  // Practically not a good idea as this function inside an ISR should be as short as possilbe
  // Serial.println(":3");
  //heartbeatPinOn2 = !heartbeatPinOn2;
  digitalWrite(heartbeatPin1,LOW);
  digitalWrite(heartbeatPin2,LOW);
}  // red stays on 1.5 sec, grn 1 sec, both off @ 2 sec in sync? Y


