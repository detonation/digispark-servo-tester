//Attiny85 / Digispark Servo tester by tawr
// Unfortunately feature creep, gold plating, and yak-shaving has taken this project
// from a neat afternoon project to a 3 day affair. I blame k.
// Designed to run off a 9v battery, but can be powered by ESC/BEC. Shunt enabled.
// Has 4 modes total:
// Switch position 0: Servo goes to center position
//    Button push: Servo sweep in 20ms/degree then returns to center
// Switch position 1: Pot controls output, for testing and controlling ESCs
//    Button push: unknown, maybe sets sensitivity of knob between 1:1 and 1:2 for precision?
// Future design considerations: transistor amplifier to run several servos.
#include <SoftRcPulseOut.h>
SoftRcPulseOut servo; 
#define REFRESH_PERIOD_MS 20 // 20ms delay between servo position updates
#define NOW               1
int centerPos = 90; //set default center position of servo for button centering
int pos = 90; // set default value for sweep
bool btn = 0; // initialized button position
bool switchPos = 0; // initialized switch position
const int switchPin = 0; // input: mode switch 
const int servoPin = 1; // servo pin
const int buttonPin = 2; // input: button
const int potPin = A0; // input: potentiometer D5 / A0

void setup() {                
  pinMode(switchPin, INPUT_PULLUP); // mode switch pin0
  pinMode(servoPin, OUTPUT); // servo pin1
  pinMode(buttonPin, INPUT_PULLUP); // mode button pin2
  pinMode(potPin, INPUT); // potentiometer D5 /A0
  servo.attach(servoPin);
  servo.setMaximumPulse(2200);  
}

void loop() {
switchPos = digitalRead(switchPin); //Read mode switch
switch (switchPos) {
  case 0:    // switch position 0
  btn = digitalRead(buttonPin);
  if (btn) { //center servo automatically
    servo.write(centerPos);
    SoftRcPulseOut::refresh(NOW); }
  else { // button pushed sweeps it full range and returns to 0
    for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
    {                                
      servo.write(pos);                // write pos 
      delay(REFRESH_PERIOD_MS);        // refresh delay
      SoftRcPulseOut::refresh(NOW);    // generates pulse
    } 
    for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
    {                                  
      servo.write(pos);                // write pos 
      delay(REFRESH_PERIOD_MS);        // refresh delay
      SoftRcPulseOut::refresh(NOW);    // generate pulse
    } 
    servo.write(centerPos); // after sweep return to center position
  }
      break;
      
  case 1:    // switch position 1
      btn = digitalRead(buttonPin); // read it, but don't do anything (yet)
      int val = analogRead(potPin); // read the pot
      int potpos = map(val, 0, 1023, 0, 180); // map range to degrees
      servo.write(potpos); // write pos
//      delay(REFRESH_PERIOD_MS); // wait after moving ? further testing required, disabled for now
      SoftRcPulseOut::refresh(NOW); // generate pulse
      break;
  }
}
