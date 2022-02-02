#include <AFMotor.h>
#include <RedBot.h>
#include <AccelStepper.h>
#define SKREFH 20  // skrefafjöldi í hring (að teknu tilliti til gírkassans)
#define SKREFL 20 // skrefafjöldi í hring (að teknu tilliti til gírkassans)

//lesid accelstepper til ad reyna ad skilja hrada styringu

float percent;
float hradi;

int pos1 = 360;
int pos2 = 567;

// Stepper motors
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
const int trigger = 5; //Necessary difference between the pResists to do something 



void forwardstep1() { 
  motor1.onestep(BACKWARD, SINGLE);
}
void backwardstep1() {  
  motor1.onestep(FORWARD, SINGLE);
}

void forwardstep2() { 
  motor2.onestep(FORWARD, SINGLE);
}
void backwardstep2() {  
  motor2.onestep(BACKWARD, SINGLE);
}


AccelStepper stepper_left(forwardstep1, backwardstep1); // use functions to step
AccelStepper stepper_right(forwardstep2, backwardstep2); // use functions to step



void setup() {
  Serial.begin( 9600 );

  delay(5000);

  //Set initial speed of the motor & stop
  stepper_left.setMaxSpeed(1000);     
  stepper_left.setSpeed(0);
  stepper_right.setMaxSpeed(1000);     
  stepper_right.setSpeed(0);
}

void loop() {
  difference = IRSensorLeft.read() - IRSensorRight.read() - 20; 

  //Decide if to turn (PID) //pid not implemented since simple left right is not working yet


  //run motors
  if(counter <1000){  //turns of the motors after x number of cycles, will be removed later

  //right turn?
   if (difference > trigger){
      stepper_left.setSpeed(500);
      stepper_right.setSpeed(10);
      //Serial.print("Right");
    }


   //left turn?
   if (difference <-trigger){
      stepper_left.setSpeed(10);
      stepper_right.setSpeed(500);
      //Serial.print("left");
    }

   //straight?
    if (-trigger <=difference && difference <=trigger){
      stepper_left.setSpeed(500);
      stepper_right.setSpeed(500);
      //Serial.print("straight");
    }

  // Make the motors run
  stepper_left.runSpeed();
  stepper_right.runSpeed();
  }
  if (counter > 1000) {
    exit(0);
  }
    
  
  
  Serial.println("        Difference: " + String(difference) );
  
  //delay(100);    // 0.01 second delay
  //Serial.print(counter);
  counter++;
}
