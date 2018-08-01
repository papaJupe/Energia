/*
 Button send 2 Serial1
 
 Board button push2 (right lower corner, pin 44) switches grn LED on  
 pin 44 (next to button) , and sends value to serial port 1
 for BLE module in central mode sending to another module
 
 
 The circuit:
 * built in grn LED attached between hidden pin 44 & ground 
 * built in pushbutton2 attached from hidden pin 42 to gnd
 * [PUSH1 is on pin 41] red LED is on pin 43 -- all hidden from user
 * internal 'pullup 10K resistor' pulls up button pin to 3.3 v
 * so it's HIGH when unpressed
 
 * on most Arduinos there is an LED attached to pin 13 but
 * MSPs have 2 onboard LED's red / grn on pins 43 & 44 

  mod AM 1803 -- corrects gross errors on original Example et. al.
 */

// constants don't change; here they set pin numbers:
const int buttonPin = 42; // PUSH2;     // pin number of the pushbutton2
const int ledPin =  44; //GREEN_LED;    // pin number of the grn LED

int buttonState = 0;     // variable for reading the pushbutton

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input with pullup:
  pinMode(buttonPin, INPUT_PULLUP);  
  Serial.begin(9600); 
  Serial1.begin(9600);  
}

void loop(){
    // read from port 1, send to port 0:
  while (Serial1.available())  // while loop sends all in buffer
  {
    int inByte = Serial1.read();
    Serial.write(inByte); 
  }
  
  // read the pushbutton voltage:
  buttonState = digitalRead(buttonPin);

  // pushbutton is HIGH if not pressed, lite is on
  if (buttonState == HIGH) {     
    // turn LED on:    
    digitalWrite(ledPin, HIGH);
    Serial1.write('1');  
  } 
  else {
    // turn LED off, button press grounds its digital pin
    digitalWrite(ledPin, LOW);
   // Serial.println("butt low");
    Serial1.write('0');
  }
  delay(100);
} // end loop
