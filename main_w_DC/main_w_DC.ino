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
int task = 0; // Which task the car is working on now. Tasks are on the facebook https://www.facebook.com/HonnunarkeppniHI/photos/pcb.4307788995937099/4307784562604209/
int last_task;  // The last task
char state = 's';
char last_state = 's';
int turn_counter = 0; // Used to make sure we keep turning when we need turning!
int counter = 0;
long startTime;
bool button_has_been_pressed = false;

// Raspberry pi communication config
const int raspi_pin = 24;

// IR sensor config
// Pin setup
RedBotSensor IRSensorLeft = RedBotSensor(A15); // initialize a sensor object on A14
RedBotSensor IRSensorRight = RedBotSensor(A14); // initialize a sensor object on A15
int difference; // The difference in the input values from the IR sensors
const int trigger = 20; //Necessary difference between the pResists to do something
int left_initial;
int right_initial;

//DC motor pin setup7
const int in1 = 35;
const int in2 = 37;
const int in3 = 39;
const int in4 = 41;
// DC speed pins
const int speedPinA = 33;
const int speedPinB = 43;


// Stepper motor config
//initiate motors
AF_Stepper motor1(SKREFH, 1);
AF_Stepper motor2(SKREFL, 2);
const int step_max_speed = 50;
const int step_turn_speed_slow = 20;//20;

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
  difference = IRSensorLeft.read() - IRSensorRight.read() - 10;
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

// Check button press for start
void check_button_press() {
  // if button has been pressed {
    button_has_been_pressed = true;
  //}
}

// Updates which task the car is dealing with
void update_task() {
  last_task = task;
  task++;
  //Serial.println("New task: " + task);
}

// Does task 1 - The light
void task1() {
  // Need to go forward, stop, push button, wait for light to turn green and then the task is complete
  Serial.println("Starting task 1");
  // Set state to GO
  set_state('g');

  // Start timer
  int timer = millis();

  // Go forward until at right position
  while ((millis() - timer) < 15000) {
    run_motors();
  }

  // Stop
  set_state('s');
  run_motors();

  // Push button

  // Wait for light

  // Task complete
  update_task();
  Serial.println("Starting task 2");
  set_state('g');
}

// Does task 2 - The hill
void task2() {
  // Just need to drive normally (do nothing, main loop takes care of line following) until the task is complete
  // Set state to GO happens in the end of task1() to decrease loop runtime

  // Check if the hill has been conquered
  //if (task complete {
    if (true) {
      update_task();
    }
}

// Does task 3 - The roundabout
void task3() {
  Serial.println("Starting task 3, currently bailing on this so nothing happens");

  //if task complete {
    update_task();
  //}
}

// Does task 4 - The finish line
void task4() {
  Serial.println("Starting task 4");
  // Set state to GO
  set_state('g');

  // Drive to end of track

  // Let the hammer fall on the button!

  //if task complete {
    //update_task();    // Commented out so that the car keeps working while testing
  //}
}

// Runs whichever task is supposed to run
void do_task(int mytask) {
  switch (mytask) {
    case 1:
      //task1();
    break;
    case 2:
      task2();
    break;
    case 3:
      task3();      
    break;
    case 4:
      task4();
    break;
    default:
      Serial.println(" All tasks done! Stopping!\n We are the champions my friends! Dun duun!");
      set_state('s');
      break;
    }
}

// SETUP
//---------------------------------------------------------------------------------------------
void setup() {
  // Begin serial communication
  Serial.begin( 9600 );

  // Timer
  startTime = millis();

  update_task();

  // Pin setup
  pinMode(raspi_pin, INPUT);

  delay(2000);

  int counter = 0; // counts cycles  so we can stop after x cycles

  // IR sensors
  left_initial = IRSensorLeft.read();
  right_initial = IRSensorRight.read();
  

  // Start steppers
  //Set initial speed of the motor & stop
  stepper_left.setMaxSpeed(step_max_speed);
  stepper_left.setSpeed(0);
  stepper_right.setMaxSpeed(step_max_speed);
  stepper_right.setSpeed(0);

  // DC motors setup
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);

  digitalWrite(in1,HIGH);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,HIGH);
  delay(2000); //test til að sjá hvað DC mótorar gera. Má stroka út seinna.


  // Wait for button press to continue
  while (!button_has_been_pressed) {
    check_button_press();
  }
}


//////////////////////////////////// fyrir DC motors
void _mRight(int pin1, int pin2) {
  digitalWrite(pin1,HIGH);
  digitalWrite(pin2,LOW);
}

void _mSetSpeed(int pinPWM, int SpeedValue) {
  analogWrite(pinPWM, SpeedValue);
}
/////////////////////////////////



// MAIN LOOP
//---------------------------------------------------------------------------------------------
void loop() {
  // Check time
  //Serial.println(millis() - startTime);
  //startTime = millis();
  
  // Check IR sensors
  difference = checkIRsensors();

  //run DC motors
  _mRight(in1,in2);
  _mRight(in3,in4);

  int speed_DC_A = 100;
  int speed_DC_B = 100;
  _mSetSpeed(speedPinA,speed_DC_A);
  _mSetSpeed(speedPinB,speed_DC_B);

  // Set state
  set_state(difference);

  //run motors
  run_motors();

  // If we have a new task, do that task
  do_task(task);

  Serial.println("        Difference: " + String(difference) );
  //Serial.println("        turn counter: " + String(turn_counter) );  

  delay(10);    // 0.01 second delay
  //Serial.print(counter);
  counter++;
  turn_counter++;   // Update turn counter
}
