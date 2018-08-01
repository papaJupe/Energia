/*
 Button built-in Example
 
 Board button push2 (right lower corner, uses hidden pin 42) 
 switches grn LED on pin 44 (next to button) 
 
 The circuit:
 * builtin grn LED attached between 44 & ground 
 * built in pushbutton2 attached from pin 42 to gnd
 * [PUSH1 is on pin 41]
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
}

void loop(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // pushbutton is HIGH if not pressed
  if (buttonState == HIGH) {     
    // turn LED on:    
    digitalWrite(ledPin, HIGH);  
  } 
  else {
    // turn LED off:
    digitalWrite(ledPin, LOW); 
  }
}
