#include <Button.h>

const int ledPin = 6;    // LED connected to digital pin 9
const int potentiometerPin = 0;

const int buttonAPin = 3;
bool lightIsOn = false;
int val;

Button button1(buttonAPin, 100); // Connect your button between pin 2 and GND

int getBrightnessFromPotentiometerValue(int value) {
  return map(value, 0, 1024, 0, 255);
}

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

  val = analogRead(potentiometerPin);
      Serial.println("val: ");
      Serial.println(val);
  int brightness = getBrightnessFromPotentiometerValue(val);
      Serial.println("brightness: ");
      Serial.println(brightness);


  if (lightIsOn == true) {
    analogWrite(ledPin, brightness); 
  } else {
     analogWrite(ledPin, 0); 
  }
}
