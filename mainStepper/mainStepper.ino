#include <AFMotor.h>
#include <RedBot.h>
#include <AccelStepper.h>
#define SKREFH 200  // skrefafjöldi í hring (að teknu tilliti til gírkassans)
#define SKREFL 200 // skrefafjöldi í hring (að teknu tilliti til gírkassans)

//initiate motors
AF_Stepper motor1(SKREFH, 1);

void forwardstep1() { 
  motor1.onestep(FORWARD, SINGLE);
}
void backwardstep1() {
  motor1.onestep(BACKWARD, SINGLE);
}

AccelStepper stepper1(forwardstep1, backwardstep1); // use functions to step

void setup() {
  delay(5000);

  //Set initial speed of the motor & stop
  stepper1.setMaxSpeed(50);     
  stepper1.setSpeed(0);
}

void loop() {
  //run motors
  stepper1.runSpeed();
}
