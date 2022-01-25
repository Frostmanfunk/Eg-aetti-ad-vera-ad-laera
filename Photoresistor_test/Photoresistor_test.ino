const int pResistorRight = A9;
const int pResistorLeft = A8;
const int ledPin = 49;

// Ljósviðnám gildi
int valueRight;
int valueLeft;
int valueLight;

void setup() {
  Serial.begin( 9600 );
  pinMode(ledPin, OUTPUT);
  pinMode(pResistorRight, INPUT);
  pinMode(pResistorLeft, INPUT);
  digitalWrite(ledPin, HIGH);
}

void loop() {
  valueRight = analogRead(pResistorRight);
  valueLeft = analogRead(pResistorLeft);

  valueLight = 500;     // Þurfum að fínstilla þetta fyrir keppni

  /*
  if (valueLight > 500 ) {
    Serial.print("Frosti segir hi");
  }
  else {
    Serial.print("Huldar");
  }
  */
  Serial.println("Hægri: " + String(valueRight) + "   Vinstri: " + String(valueLeft));
  
  delay(1000);    // 1 second delay
}
