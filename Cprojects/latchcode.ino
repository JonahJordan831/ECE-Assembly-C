#include <Servo.h>  // Include the Servo library

Servo myServo;  // Create a Servo object

void setup() {
  myServo.attach(9);  // Attach the servo to pin 9 (change the pin if necessary)
}

void loop() {
  // Move the servo from 90 to 0 degrees
  myServo.write(90); // Set the servo to 90 degrees
  delay(2000);        // Wait for 1 second

  myServo.write(0);   // Set the servo to 0 degrees
  delay(5000);        // Wait for 1 second
}
