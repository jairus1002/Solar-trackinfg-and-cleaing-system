#include <Servo.h>
int pos = 0;
// Define LDR pins
const int LDR1_PIN = A0;
const int LDR2_PIN = A1;

#define VOUT1_CONTROL_PIN 2   // Pin 1 (Vout 1 control) connected to digital pin 2
#define VOUT2_PIN A2       // Pin 2 (Vout 2) connected to analog pin A0

int relaypin =7;

// Define servo pin
const int SERVO_PIN1 = 8;
int SERVO_PIN2=9;

// Define servo limits
const int SERVO_LIMIT_HIGH = 160;
const int SERVO_LIMIT_LOW = 10;
const int SERVO_STEP = 1; // Step size for servo movement

// Define deadband to prevent vibration
const int DEADBAND = 10; // Reduced deadband for increased sensitivity

Servo servo1;
Servo servo2;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
servo2.attach(SERVO_PIN2);
pinMode(relaypin,OUTPUT);
  // Attach servo to pin
  servo1.attach(SERVO_PIN1);
   pinMode(VOUT1_CONTROL_PIN, OUTPUT); // Set Vout 1 control pin as output
   servo1.write(130);
   delay(2000);
}
void track() {
  // Read analog values from LDRs
  int ldr1Value = analogRead(LDR1_PIN);
  int ldr2Value = analogRead(LDR2_PIN);
  
  // Debugging - print values to serial monitor
  Serial.print("LDR1 Value: ");
  Serial.println(ldr1Value);
  Serial.print("LDR2 Value: ");
  Serial.println(ldr2Value);

  // Check if light is overhead and LDR values are close
  if (abs(ldr1Value - ldr2Value) < DEADBAND) { // Adjust the deadband as needed
    // Light is overhead, align solar panel horizontally
    int middleAngle = (SERVO_LIMIT_HIGH + SERVO_LIMIT_LOW) / 2;
    servo1.write(middleAngle);
    delay(1000); // Delay for stability
  }
  // Move servo gradually if light conditions are detected
  else if (ldr1Value < 18 && ldr2Value > 220) {
    // LDR1 exposed to light, LDR2 in shadow
    for (int angle = servo1.read(); angle <= SERVO_LIMIT_HIGH; angle += SERVO_STEP) {
      servo1.write(angle);
      delay(8); // Adjust delay as needed for smooth movement
    }
  }
  // Check if LDR2 is exposed to light while LDR1 is in shadow
  else if (ldr2Value < 220 && ldr1Value > 18) {
    // LDR2 exposed to light, LDR1 in shadow
    for (int angle = servo1.read(); angle >= SERVO_LIMIT_LOW; angle -= SERVO_STEP) {
      servo1.write(angle);
      delay(8); // Adjust delay as needed for smooth movement
    }
  }
  // No significant light intensity difference detected
  // Do nothing or add specific behavior here if needed

  // Delay for stability and to prevent flooding
  delay(1000);
}

void clean () {
   digitalWrite(VOUT1_CONTROL_PIN, HIGH);
  
  // Read analog value from Vout 2 pin
  int sensorValue = analogRead(VOUT2_PIN);
  
  // Print sensor value to serial monitor
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);
  
  // Turn off Vout 1 control pin
  digitalWrite(VOUT1_CONTROL_PIN, LOW);
  
  delay(1000); // Delay for stability


  if (sensorValue <400) {
    digitalWrite (relaypin,HIGH);
    delay (2000);
    
      for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
   servo2.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);       
      digitalWrite(VOUT1_CONTROL_PIN, LOW);// waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo2.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

  }
    else {
    digitalWrite(relaypin,LOW);
 servo2.write(0);
}
}



void loop() {
track ();

//clean();
}
