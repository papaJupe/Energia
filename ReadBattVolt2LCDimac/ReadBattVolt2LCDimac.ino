
/*
  ReadBattVolt2LCD - for batt volt monitor while discharge w/ volt divider on
  msp 6989 with onboard LCD; map input to [some V.] range, tone when some
  target reached, can output strings to Proc_.ino for remote monitor over USB
  or Bluetooth module on serial I/O of MSP

  old ardu Used: Liquid Crystal 8x2 display, map, analRead, expon moving
  average, round, toner
  new energia Uses: launchpad LCD, displayText, showSymbol, OneMsTaskTimer
  for delayed toning without slowing loop
*/

/*  include the library and init for msp w/ LCD (display details in mod_.cpp)
     ! IDE must select 6989 LCD board for code to compile !  */

#include "LCD_Launchpad.h"  // libs in __.app/contents/java/libraries

// LCD class instance
LCD_LAUNCHPAD myLCD;

#include <OneMsTaskTimer.h>  // enables a slow task to repeat outside 
// of loop & not slow it; used here for toning

#define PinIn A11      // analog In A14 = 30, A11 = J6 both 3.3 v. max

// float newAvg;
// float prevAvg;
// int sensorV; all local to loop
// int mapAvg;

// int sendOff = 0; // time offset to send graph app if prior discharge time
boolean toning = 0;  // whether target reached and toning is started
boolean toneOn = 1;  // 1 means toning is on

void toneOnOff();
OneMsTaskTimer_t myToner = {1000, toneOnOff, 0, 0}; // after start,
//  fx repeats every 1 sec

unsigned long starTime;  // starting time for discharge, msec, (global)


void setup()
{
  myLCD.init();
  pinMode(PinIn, INPUT);   // anal voltage input
  Serial.begin(9600);

  starTime = millis();     // start of data collection, set once
  //  Serial.print((millis() - starTime) / 60000);
  //  Serial.print(",");
  //  Serial.print("battV");
  //  Serial.print(",");
  //  Serial.println("no curr");

  //    myLCD.displayText("Hello");
  //    delay(1000);
  //    myLCD.clear();

  myLCD.showSymbol(LCD_SEG_RADIO, 1);
  //    myLCD.showSymbol(LCD_SEG_TX, 1);
  myLCD.showSymbol(LCD_SEG_RX, 1);
  delay(1400);

}  // end setup


void loop()
{
  static int count = 0; // to slow serial print to every x sec.
  myLCD.clear();
  myLCD.showSymbol(LCD_SEG_RADIO, 1);
  //  myLCD.showSymbol(LCD_SEG_TX, 1);
  myLCD.showSymbol(LCD_SEG_RX, 1);

  static float prevAvg = analogRead(PinIn) + 2; // just set once, first loop
  // int voltage;
  // read the input on analog pin
  float sensorV = analogRead(PinIn);
  delay(10);
  float newAvg = (sensorV * 0.2 + prevAvg * 0.8); // update expon moving avg of aR

  // Convert the analog reading (goes from 0 - 4095) to batt voltage (whatever_mV):
  // same vals seem to work from USB or ext 5v from VR, proto bd, or batt
    //  [for 47--10 Ω set]
  //int voltage = map(newAvg,1506,3979,1518,4014); // in mV, resolves ~5 mV
  
      // close to same map # for higher amp dc using 5.6+22=27.6 setup as
   int voltage = map(newAvg, 2404, 4060, 2415, 4092); // for 10+30=40 setup
  myLCD.displayText(String(voltage));

  // print new values to serial, if different and x sec elapsed (30+ sec wait)
  if (round(newAvg) != round(prevAvg) && count == 1)
  {
    myLCD.clear();
    delay(10);

    //  print to Ser Mon if it's on -- use for init calibration map setup
//    Serial.print("pin out= ");
//    Serial.print(newAvg);
//    Serial.print(" calc mV= ");
//    Serial.println(voltage);

    //   display raw pin avg if needed  -- dtostrf probably nonfunctional in this IDE
    //    char avgStr[4] = {'0','0','0','0'};  // make the buffer array
    //    dtostrf (newAvg, 4, 0, avgStr);   // make float into a string 4 digits long
    //               // dtostrf(sourceFLOAT,WIDTH,PRECISION,targetBUFFER)
    //    myLCD.displayText(avgStr,2);  // OK for char*, to start at raw pos 2

     myLCD.showSymbol(LCD_SEG_TX, 1);  // just TX on LCD top row when new vals sent
//    myLCD.displayText(String(voltage));  // needs Str param I guess

    printVals(voltage, 0);  // sends vals to serial port, for Proc plot, etc

    delay(100);
  } // end if changed

  if (voltage <= 2600 && toning == 0)  // disch target hit, activates toning
  {
    OneMsTaskTimer::add(&myToner); // with period as initialized
    OneMsTaskTimer::start();
    toning = 1;             // stays on until reset, no code turning off
  }  //end toning if

  count++;
  if (count == 300) count = 0;  // recycle count every x/10 sec = output interval
  prevAvg = newAvg;
  delay (100);   // ten-5 loops / second from varied delays
  
}  // end loop

void toneOnOff()
{
  if (toneOn) tone(15, 2500);   // don't need pin config for toner
  else noTone(15);  // same as P1_6
  // toneOn ^= 0x0001; // bitwise exclusive OR, inverts value on alternate calls
  toneOn = !toneOn; // tone on/off on alternate calls
}

// for serial output to Proc sketch to graph or whatever
// prints elapsed min, mV, mA
void printVals(int battV, int curr)
{ // was flaky for graphing, not now
  // Serial.print("elapsed min.= ");  // send just the 3 #'s for Proc charting
  // int time = ((millis() - starTime) / 60000);
  int time = (millis() - starTime) / 60000; // can add min if prev dc
  Serial.print(time);
  Serial.print(',');
  Serial.print(battV);
  Serial.print(',');
  Serial.println(curr);
  
}  // end printVals

//    myLCD.showSymbol(LCD_SEG_BAT_ENDS, 1);
//    delay(400);
//    myLCD.showSymbol(LCD_SEG_BAT_POL, 1);
//    delay(1000);
//    myLCD.showSymbol(LCD_SEG_BAT0, 1);
//    delay(200);
//    myLCD.showSymbol(LCD_SEG_BAT1, 1);
//    delay(200);
//    myLCD.showSymbol(LCD_SEG_BAT2, 1);
//    delay(200);
//    myLCD.showSymbol(LCD_SEG_BAT3, 1);
//    delay(200);
//    myLCD.showSymbol(LCD_SEG_BAT4, 1);
//    delay(200);
//    myLCD.showSymbol(LCD_SEG_BAT5, 1);
//    delay(200);
//    myLCD.showSymbol(LCD_SEG_BAT_POL, 1);
//    delay(1000);
//    myLCD.showSymbol(LCD_SEG_BAT0, 0);
//    delay(200);
//    myLCD.showSymbol(LCD_SEG_BAT1, 0);
//    delay(200);
//    myLCD.showSymbol(LCD_SEG_BAT2, 0);
//    delay(200);
//    myLCD.showSymbol(LCD_SEG_BAT3, 0);
//    delay(200);
//    myLCD.showSymbol(LCD_SEG_BAT4, 0);
//    delay(200);
//    myLCD.showSymbol(LCD_SEG_BAT5, 0);
//    delay(200);
//
//    myLCD.showSymbol(LCD_SEG_MINUS1, 1);
//    delay(400);
//    myLCD.showSymbol(LCD_SEG_DOT1, 1);
//    delay(400);
//    myLCD.showSymbol(LCD_SEG_COLON2, 1);
//    delay(400);
//    myLCD.showSymbol(LCD_SEG_DOT2, 1);
//    delay(400);
//    myLCD.showSymbol(LCD_SEG_DOT3, 1);
//    delay(400);
//    myLCD.showSymbol(LCD_SEG_COLON4, 1);
//    delay(400);
//    myLCD.showSymbol(LCD_SEG_DOT4, 1);
//    delay(400);
//    myLCD.showSymbol(LCD_SEG_DOT5, 1);
//    delay(400);
//    myLCD.showSymbol(LCD_SEG_DEG5, 1);
//    delay(1000);
//  myLCD.displayText("ABCDE");
//  delay(1000);
//
//  myLCD.clear();
//  myLCD.showChar('1', 1);
//  myLCD.clear();
//  myLCD.print("543");
//  delay(1000);
//  myLCD.println("21");
//  delay(1000);
//  myLCD.print("XYZ\nxyz");
//  delay(1000);
//  myLCD.println("");
//  myLCD.clear();
