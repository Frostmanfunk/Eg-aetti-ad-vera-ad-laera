#include <AFMotor.h>
#include <RedBot.h>
#include <AccelStepper.h>
#define SKREFH 2000  // skrefafjöldi í hring (að teknu tilliti til gírkassans)
#define SKREFL 2000 // skrefafjöldi í hring (að teknu tilliti til gírkassans)

//lesid accelstepper til ad reyna ad skilja hrada styringu

const int pResistorRight = A9;
const int pResistorLeft = A8;
const int ledPin = 49;
float percent;
float hradi;

int pos1 = 360;
int pos2 = 567;


//initiate motors
AF_Stepper motor1(SKREFH, 1);
AF_Stepper motor2(SKREFL, 2);

// new line sensors
RedBotSensor IRSensorLeft = RedBotSensor(A15); // initialize a sensor object on A14
RedBotSensor IRSensorRight = RedBotSensor(A14); // initialize a sensor object on A15

// this is old 
// Ljósviðnám gildi
//int valueRight;
//int valueLeft;


int difference;

int counter =0; // counts cycles  so we can stop after x cycles
//
int trigger = 10; //Necessary difference between the pResists to do something 



void forwardstep1() { 
  motor1.onestep(FORWARD, SINGLE);
}
void backwardstep1() {  
  motor1.onestep(BACKWARD, SINGLE);
}

void forwardstep2() { 
  motor2.onestep(BACKWARD, SINGLE);
}
void backwardstep2() {  
  motor2.onestep(FORWARD, SINGLE);
}


AccelStepper stepper1(forwardstep1, backwardstep1); // use functions to step
AccelStepper stepper2(forwardstep2, backwardstep2); // use functions to step



void setup() {
  Serial.begin( 9600 );
  pinMode(ledPin, OUTPUT);
  pinMode(pResistorRight, INPUT);
  pinMode(pResistorLeft, INPUT);
  digitalWrite(ledPin, HIGH);

  delay(5000);

  //Set initial speed of the motor & stop
  stepper1.setMaxSpeed(50);     
  stepper1.setSpeed(0);
  stepper2.setMaxSpeed(500);     
  stepper2.setSpeed(0);
}

void loop() {
  //read lights
//  valueRight = analogRead(pResistorRight); //old sensors
//  valueLeft = analogRead(pResistorLeft);   //old sensors
  difference = IRSensorLeft.read() - IRSensorRight.read(); 

  //Decide if to turn (PID) //pid not implemented since simple left right is not working yet


  //run motors
  if(counter <2000){  //turns of the motors after x number of cycles, will be removed later

  
  

  
  
  //right turn?
   if (difference > trigger){
      stepper1.setSpeed(50);
      stepper2.setSpeed(-50);
      Serial.print("Right");
    }


   //left turn?
   if (difference <-trigger){
      stepper1.setSpeed(-50);
      stepper2.setSpeed(50);
      Serial.print("left");
    }

   //straight?
    if (-trigger <=difference &&difference <=trigger){
       stepper1.setSpeed(50);
      stepper2.setSpeed(50);
       Serial.print("straight");
    }

//  stepper1.runSpeed();
//  stepper2.runSpeed();
  }
    
  
  
  Serial.println("        Difference: " + String(difference) );
  
  //delay(100);    // 0.01 second delay
  Serial.print(counter);
  counter++;
}
