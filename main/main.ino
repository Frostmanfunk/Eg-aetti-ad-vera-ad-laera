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
#include <Servo.h>

Servo myservoFront;
Servo myservoBack;

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
int timer;

//afturmotorapinnar
int in1 = 31;
int in2 = 33;
int in3 = 35;
int in4 = 37 ;
int speedPinA = 7;
int speedPinB = 12;

// Raspberry pi communication config
const int raspi_pin = 24;

// IR sensor config
// Pin setup
RedBotSensor IRSensorLeft  = RedBotSensor(A15); // initialize a sensor object on A14
RedBotSensor IRSensorRight = RedBotSensor(A14); // initialize a sensor object on A15
int difference; // The difference in the input values from the IR sensors
const int trigger = 30; //Necessary difference between the pResists to do something // 20 for comp
int left_initial;
int right_initial;




// Stepper motor config
//initiate motors
AF_Stepper motor1(SKREFH, 1);
AF_Stepper motor2(SKREFL, 2);
const int step_max_speed = -50;
const int step_turn_speed_slow = 50;//20;

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


void RearRightON() {
  digitalWrite(in2, HIGH);
  digitalWrite(in1, LOW);
}

void RearLeftON() {
  digitalWrite(in4, HIGH);
  digitalWrite(in3, LOW);
}

void RearRightBack() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}

void RearLeftBack() {
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void RearDriveOn() {
  digitalWrite(in2, HIGH);
  digitalWrite(in1, LOW);
  digitalWrite(in4, HIGH);
  digitalWrite(in3, LOW);
}


void RearDriveOff() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
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
        //                RearDriveOff();
        break;
      case 'g':
        //GO
        stepper_left.setSpeed(step_max_speed);
        stepper_right.setSpeed(step_max_speed);
        //                RearDriveOff();
        break;
      case 'l':
        // TURN LEFT
        stepper_left.setSpeed(step_turn_speed_slow);
        stepper_right.setSpeed(step_max_speed - 10);
        //                RearDriveOff();
        //                RearLeftON();
        //        RearRightBack();
        break;
      case 'r':
        stepper_left.setSpeed(step_max_speed - 10);
        stepper_right.setSpeed(step_turn_speed_slow);
        //                RearDriveOff();
        //                RearRightON();
        //        RearLeftBack();
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
  difference = IRSensorLeft.read() - IRSensorRight.read() + 20;
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

  // Go forward until at right position
  timer = millis() / 1000;
  while ((millis() / 1000 - timer) <= 3) {
    //if (millis() / 1000 <= 15) {
    state = 'g';
    run_motors();
    //Serial.println("go");
    //Serial.println(state);
    //}
  }



  // Stop
  state = 's';
  run_motors();
  myservoFront.write(0);
  delay(3000);
  
  // Push button - stilll missing
  // Wait for light
  timer = millis() / 1000;
  Serial.println("waiting for light");
  while ((millis() / 1000 - timer) <= 20) {
    if (digitalRead(raspi_pin) == HIGH); {
      Serial.println("Raspi");
      break;
    }

  }

  // Task complete
  Serial.println("Starting task 2");
  set_state('g');
  update_task();
  timer = millis() / 1000;
}
//  Serial.println("Starting task 2");
//  set_state('g');


// Does task 2 - Drive to The hill
void task2() {
  // Just need to drive normally (do nothing, main loop takes care of line following) until the task is complete
  // Set state to GO happens in the end of task1() to decrease loop runtime

  // Check if the hill has been conquered
  //if (task complete {
  if ((millis() / 1000 - timer) >= 172) {
    Serial.println("Task 2 done");
    update_task();
  }
  else {
    if ((millis() / 1000 - timer) >= 22) {
      RearDriveOff();
    }
    else {

      RearDriveOn();
      Serial.println("Task 2 Started");
      state = 'g';


    }
  }
}


// Does task 3 -  The roundabout
void task3() {
  //  Serial.println("Starting task 3, currently bailing on this so nothing happens");
  //  RearDriveOn();
  //  if ((millis() / 1000 - timer) > 150) {
  //    RearDriveOff();
  update_task();
  //  }
}


// Does task 4 - The finish line
void task4() {
  Serial.println("Starting task 4, currently bailing on this so nothing happens");

  myservoBack.write(30);
  delay(5000);
  exit(0);

  // Drive to end of track

  // Let the hammer fall on the button!

  //if task complete {
  update_task();    // Commented out so that the car keeps working while testing
  //}
}

// Runs whichever task is supposed to run
void do_task(int mytask) {
  switch (mytask) {
    case 1:
      task1();
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
      exit(0);
      break;
  }
}






// SETUP
//---------------------------------------------------------------------------------------------
void setup() {
  // Begin serial communication
  Serial.begin( 9600 );

  // Timer
  //startTime = millis();

  update_task();
  //update_task(); // taka essa linu

  // Pin setup
  pinMode(raspi_pin, INPUT);



  int counter = 0; // counts cycles  so we can stop after x cycles

  // IR sensors
  left_initial = IRSensorLeft.read();
  right_initial = IRSensorRight.read();


  //Servo
  myservoFront.attach(9);
  myservoBack.attach(10);
  myservoBack.write(89);
  myservoFront.write(90);


  // Start steppers
  //Set initial speed of the motor & stop
  stepper_left.setMaxSpeed(step_max_speed);
  stepper_left.setSpeed(0);
  stepper_right.setMaxSpeed(step_max_speed);
  stepper_right.setSpeed(0);

  // DC motors setup
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, HIGH);
  //delay(2000); //test til að sjá hvað DC mótorar gera. Má stroka út seinna.

  delay(5000);
  // Wait for button press to continue
  while (!button_has_been_pressed) {
    check_button_press();
  }
}





/////////////////////////////////



// MAIN LOOP
//long startTime1 = millis();
//---------------------------------------------------------------------------------------------
void loop() {
  // Check time
  //Serial.println(millis() - startTime);
  //startTime = millis();

  // Check IR sensors
  difference = checkIRsensors();


  // Set state
  set_state(difference);

  //run motors


  // If we have a new task, do that task
  do_task(task);
  run_motors();


  //Serial.println("        Difference: " + String(difference) );
  //Serial.println("        turn counter: " + String(turn_counter) );

  //delay(10);    // 0.01 second delay

  //  Serial.println(millis() / 1000);

  //if(millis()/1000 ==100){RearDriveOn();}
  //if(millis()/1000 ==150){RearDriveOff();}


  counter++;
  turn_counter++;   // Update turn counter
}
