#define IRPIN 2     // Digital pin connected to the IR sensor

// Connect pin 1 (on the left) of the sensor to your defined IRPIN
// Connect pin 2 of the sensor to GROUND
// Connect pin 3 (on the right) of the sensor to +5V

void setup() { 
  pinMode (IRPIN, INPUT); // sensor pin INPUT 
  Serial.begin(9600);
  Serial.println("IR readings:");
}

void loop() {
  delay(1000);

  // Read IR status
  int statusSensor = digitalRead (IRPIN);

  if (statusSensor == 1) {
    Serial.println("No person detected.\n");
  } 
  else {
    Serial.println("PERSON DETECTED!\n");
  }
  
}
