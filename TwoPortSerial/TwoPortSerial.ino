/*
  2 port serial
   [to upload I need to use second, higher USBmodem port on Mac]
  Receives from the main USB serial port(0), sends to Serial1.
  AND receives from serial port 1, sends to the main USB (Serial0).

  Wire:
    Any serial device attached to Serial port 1, (3,4) on 5529
    Serial monitor open on Serial port 0:

  created 30 Dec. 2008 by Tom Igoe
  mod 1803 AM for test of BLE modules: wire HM10 et.al. to Ser1
  3.3 v. I/O, pin 3,4 = rx/tx, setup module w/ AT cmds over USB as
  central (or periph for phone app test), try comm w/ other
  module on Ardu Mega or something -- sending from mod as central
  fails, recv OK from peripheral

*/

void setup() {
  // initialize both serial ports:
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  char serIn[10] = {0}; // inits empty array for serial input
  // read from port 1, send to port 0:
  int usByte;
  while (Serial1.available())  // while sends all in buffer
  {
    int inByte = Serial1.read();
    Serial.write(inByte);
  }

  // read from ser mon, write to serial1
  while (Serial.available()) // while sends all chars in buffer
  {
    usByte = Serial.read();
    Serial1.write(usByte);

    // ALL input bytes --> inByt char array, send fails if too big
    //    Serial.readBytes(serIn, Serial.available());
    //    Serial.print(".print input ");
    //    Serial.println(serIn);  // input bytes print as chars
    //    //Serial.println();
    //    delay(10);
    //    Serial1.write(serIn);
  }  // end while
  delay(50);
}  // end loop
