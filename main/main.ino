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
#include <thread> // Til að multithreada stepper motora
#define SKREFH 2000  // skrefafjöldi í hring (að teknu tilliti til gírkassans)
#define SKREFL 2000 // skrefafjöldi í hring (að teknu tilliti til gírkassans)

// Raspberry pi communication config
const int raspi_pin = 24


// IR sensor config
// Pin setup
  RedBotSensor IRSensorLeft = RedBotSensor(A15); // initialize a sensor object on A14
  RedBotSensor IRSensorRight = RedBotSensor(A14); // initialize a sensor object on A15
int difference; // The difference in the input values from the IR sensors
const int trigger = 5; //Necessary difference between the pResists to do something 


// Stepper motor config
//initiate motors
AF_Stepper motor1(SKREFH, 1);
AF_Stepper motor2(SKREFL, 2);
const int step_max_speed = 1000;
const int step_turn_speed_slow = 50;
// Set state - Used for knowing what the motor wants to do.
// 's' for STOP, 'g' for GO, 'l' for LEFT, 'r' for RIGHT
char left_state = 's';
char right_state = 's';

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

// Sets the state of both of the stepper motors to 
void set_state(char state_in) {
  left_state = state_in;
}

// Loop for stepper thread
// Stepper is the motor, state is the motor state, side is which side (0 for left and 1 for right)
void stepper_loop(stepper, int side) {
  char state;
  char last_state = 's';

  //Set initial speed of the motor & stop
  stepper.setMaxSpeed(step_max_speed);
  stepper.setSpeed(0);

  // The loop
  while(1) {
    if (side) {
      // Right stepper
      state = right_state;
    }
    else {
      // Leftstepper
      state = left_state
    }
    if (last_state != state) {
      switch (state)
      {
      case 's':
        //STOP
        stepper.setSpeed(0);
        break;
      case 'g':
        //GO
        stepper.setSpeed(step_max_speed);
        break;
      case 'l':
        // TURN LEFT
        if (side) {
          // Right stepper
          stepper.setSpeed(step_max_speed);
        }
        else {
          //left stepper
          stepper.setSpeed(step_turn_speed_slow);
        }
        break;    
      case 'r':
        //TURN RIGHT
        if (side) {
          // Right stepper
          stepper.setSpeed(step_turn_speed_slow);
        }
        else {
          //left stepper
          stepper.setSpeed(step_max_speed);
        }
        break;
      default:
        // Do nothing
        break;
      }
    }
    // Run motor
    stepper.runSpeed();
    // Update last state
    last_state = state;
  }
}

// SETUP
//---------------------------------------------------------------------------------------------
void setup() {
  Serial.begin( 9600 );

  // Pin setup
  pinMode(raspi_pin, INPUT);

  delay(5000);

  int counter = 0; // counts cycles  so we can stop after x cycles


  // Start steppers
  std::thread left_thread (stepper_loop, stepper_left, left_state, 0);
  std::thread right_thread (stepper_loop, stepper_right, right_state, 1);
  left_thread.join();
  right_thread.join();
}

// MAIN LOOP
//---------------------------------------------------------------------------------------------
void loop() {
  difference = IRSensorLeft.read() - IRSensorRight.read() - 20; 

  //Decide if to turn (PID) //pid not implemented since simple left right is not working yet


  //run motors
  if(counter <1000){  //turns of the motors after x number of cycles, will be removed later

  //right turn?
   if (difference > trigger){
     set_state('r');
    }

   //left turn?
   if (difference <-trigger){
     set_state('l');
    }

   //straight?
    if (-trigger <=difference && difference <=trigger){
      set_state('g');
    }

  // Make the motors run
  stepper_left.runSpeed();
  stepper_right.runSpeed();
  }
  if (counter > 1000) {
    exit(0);
  }
  
  //Serial.println("        Difference: " + String(difference) );
  
  //delay(100);    // 0.01 second delay
  //Serial.print(counter);
  counter++;
}
