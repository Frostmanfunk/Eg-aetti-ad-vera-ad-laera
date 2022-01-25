#include <AFMotor.h>
#include <Servo.h>

const int pResistorRight = A9;
const int pResistorLeft = A8;
const int ledPin = 49;

//initiate motors
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);

// Ljósviðnám gildi
int valueRight;
int valueLeft;
int difference;

int counter =0; // counts cycles  so we can stop after x cycles
//
int trigger = 10; //Necessary difference between the pResists to do something 

void setup() {
  Serial.begin( 9600 );
  pinMode(ledPin, OUTPUT);
  pinMode(pResistorRight, INPUT);
  pinMode(pResistorLeft, INPUT);
  digitalWrite(ledPin, HIGH);

  //Set initial speed of the motor & stop
  motor1.setSpeed(100);
  motor2.setSpeed(100);
  motor2.run(RELEASE);
  motor1.run(RELEASE);
}

void loop() {
  //read lights
  valueRight = analogRead(pResistorRight);
  valueLeft = analogRead(pResistorLeft);
  difference = valueRight - valueLeft; 

  //Decide if to turn (PID) //pid not implemented since simple left right is not working yet


  //run motors
  if(counter <2){  //turns of the motors after x number of cycles, will be removed later
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  }
  else{
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    }
  
  //right turn?
   if (difference > trigger){
      motor1.setSpeed(200);
      motor2.setSpeed(00);
      Serial.print("Right");
    }


   //left turn?
   if (difference <-trigger){
      motor1.setSpeed(0);
      motor2.setSpeed(200);
      Serial.print("left");
    }

   //straight?
    if (-trigger <=difference &&difference <=trigger){
       motor1.setSpeed(100);
       motor2.setSpeed(100);
       Serial.print("straight");
    }
  
  
  Serial.println("        Difference: " + String(difference) );
  
  delay(100);    // 0.01 second delay
  Serial.print(counter);
  counter++;
}
