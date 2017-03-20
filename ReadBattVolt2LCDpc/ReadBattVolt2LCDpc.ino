
/*
  ReadBattVolt2LCD - for rough batt monitor while discharge w/ volt divider, with msp 6989
  with LCD on board; map scaled down input to 11.5-13 v range, tone when some target reached,
  can output N str to Proc .exe for remote monitor

  old ardu Uses: Liquid Crystal 8x2 display, map, analRead, expon moving average, round, toner
  new energia uses: launchpad LCD, displayText, showSymbol
*/

//  include the library and init for msp w/ LCD
#include "LCD_Launchpad.h"   // display details in .cpp

// LCD class instance
LCD_LAUNCHPAD myLCD;

#include <OneMsTaskTimer.h>  // enables a slow task to repeat outside 
// of loop & not slow it; used here for toning

#define PinIn A11      // analog pin In

// float newAvg;
// float prevAvg;
// int sensorV; all local to loop
// int mapAvg;
boolean toning = 0;  // whether target reached and toning has started
boolean status = 1;  // whether to play tone or not
void toneOnOff();

OneMsTaskTimer_t myToner = {1000, toneOnOff, 0, 0}; // after start, fx repeats every 1 sec

unsigned long starTime;  // starting time for discharge, msec, (global)


void setup()
{
  myLCD.init();
  pinMode(6, INPUT);   // same as A11
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

}  // end setup


void loop()
{
  static int count = 0; // to print w/ interval
  myLCD.showSymbol(LCD_SEG_RADIO, 1);
  //    myLCD.showSymbol(LCD_SEG_TX, 1);
  myLCD.showSymbol(LCD_SEG_RX, 1);
  static float prevAvg = analogRead(PinIn) + 2; // just set once, first loop
  int voltage;
  // read the input on analog pin
  float sensorV = analogRead(PinIn);
  delay(10);
  float newAvg = (sensorV * 0.2 + prevAvg * 0.8); // update expon moving average of AR

  // Convert the analog reading (which goes from 0 - 4095) to batt voltage (11500-13000mV):
  // these vals seem to work from USB or ext 5v from VR powered from batt
  voltage = map(newAvg, 2800, 3330, 11000, 13000); // in mV, resolves ~5 mV

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

  // print new values to LCD and serial, if different, and 3 sec have elapsed
  if (round(newAvg) != round(prevAvg) && count == 1)
  {
    myLCD.clear();
    delay(50);

    //print to Ser Mon if it's on
    //        Serial.print("pin out= ");
    //        Serial.print(newAvg);
    //        Serial.print(" calc mV= ");
    //        Serial.println(voltage);

    //   display raw pin avg if needed
    //    char avgStr[4] = {'0','0','0','0'};  // make the buffer array
    //    dtostrf (newAvg, 4, 0, avgStr);   // make float into a string 4 digits long
    //               // dtostrf(sourceFLOAT,WIDTH,PRECISION,targetBUFFER)
    //    myLCD.displayText(avgStr,2);  // OK for char*, to start at raw pos 2
    myLCD.displayText(String(voltage));  // needs Str param I guess
    delay(50);
    printVals(voltage, 0);  // sends vals to serial port
    //myLCD.println(voltage);
    prevAvg = newAvg;
  } // end if changed

  if (voltage <= 12400 && toning == 0)  // target for 12 v dc
  {
    OneMsTaskTimer::add(&myToner); // with period as initialized
    OneMsTaskTimer::start();
    toning = 1;
  }  //end toning if

  count++ == 1;
  if (count == 15) count = 0;  // recycle count every 3 sec
  delay (100);   // five loops / second
}  // end loop

void toneOnOff()
{ 
  if (status) tone(8, 2700);   // don't need pin config for toner
  else noTone(8);
  // status ^= 0x0001; // bitwise exclusive OR, inverts value on alternate calls
  status = !status; // tone on/off on alternate calls
}

// for serial output to Proc sketch
// prints elapsed min, mV, mA, resets prevTime
void printVals(int battV, int curr)
{ // this is fine for rough output, flaky for graphing due to irreg. sends @ startup
  Serial.print("elapsed min.= ");  // use just 3 strings for Proc charting
  int time = ((millis() - starTime) / 60000);
  // delay(2);  didn't help w/ startup irreg.
  Serial.print((int)time);
  Serial.print(',');
  Serial.print(battV);
  Serial.print(',');
  Serial.println("noCurr");
  //  prevTime = millis();
}  //end printVals

