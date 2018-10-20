const int buzzerPin = 12;
int speakerPin = 12;
 
int numTones = 10;
int tones[] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440};

void setup() {
  // put your setup code here, to run once:
  pinMode(buzzerPin, OUTPUT);
//
//    for (int i = 0; i < numTones; i++)
//  {
//    tone(speakerPin, tones[i]);
//    delay(500);
//  }
//  noTone(speakerPin);

  
}
int waitTime = 0;
int counter = 0;
void loop() {
  counter++;
  if (counter % 200 <= 150) {
    waitTime = 1;
  } else {
    waitTime = 4;
  }
digitalWrite(buzzerPin, HIGH);
delay(500);
digitalWrite(buzzerPin, LOW);
delay(waitTime);
delay(1000);

}
