/*
  Voltage Sample Calib --  (moving) averages analog pin input, 0-3.3 v. (as from batt
  discharger device with an op-amp level shifting comparator + amp);
  maps that to the 11.5-13.0 v. being sampled; prints raw and calcul. voltage
  if it's changed; use to get params for map function in other sketches
*/

#define pinIn 2  // A10 on MPS430

float newAvg;
float prevAvg;
float sensorV;
float mapAvg;

// setup runs once when you power on or Reset
void setup()
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(pinIn, INPUT);
  prevAvg = analogRead(pinIn);  // just some starting place
}

//  loop runs over and over
void loop()
{ static int count = 0; // set on 1st loop, used to slow printing freq. to every 2 sec
  // read the input on analog pin
  sensorV = analogRead(pinIn);
  delay(10);
  newAvg = (sensorV * 0.2 + prevAvg * 0.8); //update expon moving average of AR; using float and mult improves accuracy of this calc.
  // convert analog reading (0 - 4096) to a voltage (11.5-13.0) using map
  mapAvg = round(newAvg);   // nearest int good for mapping
  int voltage = map(mapAvg, 480, 3800, 11500, 13000); // in mV, resolves ~1 mV

  // print new values every 2 sec. if changed
  if (mapAvg != round(prevAvg) && count == 1)
  {
    Serial.print("pin out= "); Serial.print(mapAvg);
    Serial.print("  volt calc= "); Serial.println(voltage);

  }
  prevAvg = newAvg;
  count++;
  if (count == 20) count = 0;
  delay (100);   // __ loops / second
}



