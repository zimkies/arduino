//#include <ButtonDebounce.h>
#include <Button.h>

bool lightIsOn = false;
//ButtonDebounce button(8, 50);
Button button(8, 100);
void setup() {
  Serial.begin(9600);
}

void loop() {
//  Serial.println("Looped");

//  button.update();
//  if(button.state() == LOW){
//    Serial.println("Clicked");
//    lightIsOn = not lightIsOn;
//  }

  if ( button.toggled() ) {
    Serial.println("Clicked");
    lightIsOn = not lightIsOn;
  }

  


}
