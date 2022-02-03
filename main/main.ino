/*
  Project: Kóði fyrir hönnunarkeppni HÍ vorið 2022
  Skrifað af Ég ætti að vera að læra hópnum.
  Github repo: https://github.com/Frostmanfunk/Eg-aetti-ad-vera-ad-laera

  Notes:
   - lesid accelstepper til ad reyna ad skilja hrada styringu
*/

#include <AFMotor.h>
#include <RedBot.h>
#include <AccelStepper.h>
#define SKREFH 200  // skrefafjöldi í hring (að teknu tilliti til gírkassans)
#define SKREFL 200 // skrefafjöldi í hring (að teknu tilliti til gírkassans)

// Car config
// Set state - Used for knowing what the motor wants to do.
// 's' for STOP, 'g' for GO, 'l' for LEFT, 'r' for RIGHT
char state = 's';
char last_state = 's';
int turn_counter = 0; // Used to make sure we keep turning when we need turning!
int counter = 0;
long startTime;

// Raspberry pi communication config
const int raspi_pin = 24;

// IR sensor config
// Pin setup
RedBotSensor IRSensorLeft = RedBotSensor(A15); // initialize a sensor object on A14
RedBotSensor IRSensorRight = RedBotSensor(A14); // initialize a sensor object on A15
int difference; // The difference in the input values from the IR sensors
const int trigger = 20; //Necessary difference between the pResists to do something


// Stepper motor config
//initiate motors
AF_Stepper motor1(SKREFH, 1);
AF_Stepper motor2(SKREFL, 2);
const int step_max_speed = 50;
const int step_turn_speed_slow = -10;//20;

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

// Loop for stepper thread
// Stepper is the motor, state is the motor state, side is which side (0 for left and 1 for right)
void run_motors() {
  if (last_state != state) {
    switch (state)
    {
      case 's':
        //STOP
        stepper_left.setSpeed(0);
        stepper_right.setSpeed(0);
        break;
      case 'g':
        //GO
        stepper_left.setSpeed(step_max_speed);
        stepper_right.setSpeed(step_max_speed);
        break;
      case 'l':
        // TURN LEFT
        stepper_left.setSpeed(step_turn_speed_slow);
        stepper_right.setSpeed(step_max_speed);
        break;
      case 'r':
        stepper_left.setSpeed(step_max_speed);
        stepper_right.setSpeed(step_turn_speed_slow);
        break;
      default:
        // Do nothing
        break;
    }
  }
  // Run motor
  stepper_left.runSpeed();
  stepper_right.runSpeed();
}

// Function that checks the IR sensors and returns the difference between the values, offset
int checkIRsensors() {
  difference = IRSensorLeft.read() - IRSensorRight.read() - 20;
  return difference;
}

// Sets the state of the car given the difference of the IR sensors measurement
void set_state(int difference) {
  // Update last state
  last_state = state;
  //right turn?
  if (difference > trigger) {
    state = 'r';
    turn_counter = 0;
  }
  //left turn?
  if (difference < -trigger) {
    state = 'l';
    turn_counter = 0;
  }
  //straight?
  if (-trigger <= difference && difference <= trigger) {
    state = 'g';
  }

  // Keep turning if we need to
  //if (last_state == 'l' || last_state == 'r') {
    //if (turn_counter < 200) {
      //state = last_state;
    //}
  //}

}

// SETUP
//---------------------------------------------------------------------------------------------
void setup() {
  Serial.begin( 9600 );
  startTime = millis();
  //state = 'r';

  // Pin setup
  pinMode(raspi_pin, INPUT);

  delay(2000);

  int counter = 0; // counts cycles  so we can stop after x cycles

  // Start steppers
  //Set initial speed of the motor & stop
  stepper_left.setMaxSpeed(step_max_speed);
  stepper_left.setSpeed(0);
  stepper_right.setMaxSpeed(step_max_speed);
  stepper_right.setSpeed(0);
}

// MAIN LOOP
//---------------------------------------------------------------------------------------------
void loop() {
  
  // Check IR sensors
  difference = checkIRsensors();

  // Set state
  set_state(difference);

  //run motors
  if (counter < 5000) { //turns of the motors after x number of cycles, will be removed later

    // Make the motors run
    run_motors();
  }
  if (counter > 5000) {
    Serial.print("Exiting");
    exit(0);
  }

  //Serial.println("        Difference: " + String(difference) );
  //Serial.println("        turn counter: " + String(turn_counter) );
  

  delay(10);    // 0.01 second delay
  //Serial.print(counter);
  counter++;
  turn_counter++;   // Update turn counter
}
