const int buttonPin = 10; //decide which pin to use later for on/off button

int buttonState = 0;      // variable for reading the pushbutton status


void setup() {
  pinMode(buttonPin, INPUT);

}

void loop() {
  buttonState = digitalRead(buttonPin);
}

bool buttonFunction (int buttonState) {
  if (buttonState == HIGH) {
    // start / keep arduino program running
  } else {
    // stop arduino program
  }
}
