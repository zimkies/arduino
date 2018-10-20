const int ledPin = 3;    // LED connected to digital pin 9

const int buttonAPin = 8;
bool lightIsOn = false;

#include <Button.h>

Button button1(buttonAPin, 100); // Connect your button between pin 2 and GND


void setup() {
  button1.begin();
  pinMode(ledPin, OUTPUT);  

  Serial.begin(9600);
}

void loop() {
  if (button1.pressed()) {
    Serial.println("Button 1 pressed");
    lightIsOn = not lightIsOn;
  }

  if (lightIsOn == true) {
    digitalWrite(ledPin, HIGH); 
  } else {
     digitalWrite(ledPin, LOW); 
  }
}
