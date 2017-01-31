
//   VoltSampleCurrentCalib adapt from ardu's 
//   reads analog input on pin X, and prints the result
//   to the serial monitor if it has changed; uses mapping values to map 
//   pin AR to actual current (voltage across 2 ohm) going to op-amp A.
//   Uses: analRead, expon moving averag, map, round

float newAvg;
float prevAvg;          // globals, appear in setup and loop
float indxAvg;

float pinSees;
 // int current;

#define PinIn A2

// setup runs once when you power on or press reset button
void setup() 
{
  Serial.begin(9600);
  pinMode(25, INPUT);   // same as A2 on msp430
  prevAvg = analogRead(PinIn);  // just some starting place for the MA
  indxAvg = prevAvg;          // init this value for MA calc in loop

}

void loop() 
{
  // read the input on analog pin, use moving average to stabilize 
  pinSees = analogRead(PinIn);
  delay(10);
  newAvg = pinSees*0.2 + prevAvg*0.8;  // update expon moving average of AR
 // map AR ticks to current (mA) 
 // convert analog reading (0 - 4096) to current (via amplified voltage (0-1.9 across Rc) 

//int current = map(newAvg,2316,3970,500,750);  //  Rl 10 Rc 2+10 goes to 950
//current = map(newAvg,632,924,650,880);  //  Rl 8.6
//int current = map(newAvg,1940,3560,450,700);  //  Rl 14.8  Rc 2+10
// int current = map(newAvg,210,656,300,650);  // with Rc 2, Rl 16
 // int current = map(newAvg,870,1640,250,350);  // with Rc 2, Rl 30
int current = map(newAvg,880,2040,250,400); // for 2.0 ohm Rc, R1 26 ohm
// int current = map(newAvg,160,330,200,300);  //with 2.7 Rc and 35 Rl


    prevAvg = newAvg;

  // print out the new MA if changed from index value, set new index
  if (round(newAvg) != round(indxAvg))
  {
    Serial.print("pin out= "); 
    Serial.print(newAvg,0); // just want the integer part
    Serial.print(" calc curr = "); 
    Serial.println(current);
    indxAvg = newAvg;
  }
  delay (200);   // five loops / second
}




