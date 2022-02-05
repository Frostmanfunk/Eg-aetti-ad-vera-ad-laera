#include <Servo.h>

Servo myservoFront;
Servo myservoBack;

void setup() {
  // put your setup code here, to run once:
  myservoFront.attach(9);
  myservoBack.attach(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  //myservoFront.write(0);
  myservoBack.write(30);
  delay(5000);
  myservoBack.write(89);
  //myservoFront.write(90);
  delay(3000);
}
