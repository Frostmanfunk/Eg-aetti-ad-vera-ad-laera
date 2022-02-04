/*
int in1 = 35;
int in2 = 37;
int in3 = 39;
int in4 = 41 ;

int speedPinA = 33;
int speedPinB = 43;
*/

int in1 = 31;
int in2 = 33;
int in3 = 35;
int in4 = 37 ;

int speedPinA = 7;
int speedPinB = 12;


void setup() {
  // afturmotorar
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);

  digitalWrite(in1,HIGH);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.write("HA");
  delay(1000);
  RearDriveOn();
  delay(1000); //test má taka
  RearDriveOff();
}

void RearDriveOn() {
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
}

void RearDriveOff() {
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
}
