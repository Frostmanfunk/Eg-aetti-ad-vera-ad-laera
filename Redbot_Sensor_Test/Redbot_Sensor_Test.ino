#include <RedBot.h>
RedBotSensor IRSensorLeft = RedBotSensor(A14); // initialize a sensor object on A14
RedBotSensor IRSensorRight = RedBotSensor(A15); // initialize a sensor object on A15

void setup()
{
  Serial.begin(9600);
  Serial.println("Welcome to experiment 6!");
  Serial.println("------------------------");
}

void loop()
{
  Serial.print("IR Sensor Readings: ");
  Serial.print(IRSensorLeft.read()); 
  Serial.print("\t");  // tab character
  Serial.print(IRSensorRight.read());
  Serial.println();
  delay(100);
}
