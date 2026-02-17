
#include <Servo.h>

Servo myservo;  // create Servo object to control a servo

int pos = 0;    // variable to store the servo position

int Red = 5;
int Grn = 6;
int Blu = 11;

void setup() {
  myservo.attach(3);  // attaches the servo on pin 6 to the Servo object
  Serial.begin(9600);
  pinMode(Red, OUTPUT);
  pinMode(Grn, OUTPUT);
  pinMode(Blu, OUTPUT);
}

void loop() {
  int redBrightness = 0;
  int greenBrightness = 0;
  int blueBrightness = 0;
  
  // Read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  
  // Print out the value you read:
  Serial.println(sensorValue);

  // Determine LED colors based on sensorValue ranges
  if (sensorValue < 320) {
    // Red region for values under 320
    redBrightness = 255;
    greenBrightness = 0;
    blueBrightness = 0;
  } 
  else if (sensorValue >= 320 && sensorValue < 350) {
    // Transition region from red to green
    redBrightness = map(sensorValue, 320, 350, 255, 0); // Gradually decrease red
    greenBrightness = map(sensorValue, 320, 350, 0, 255); // Gradually increase green
    blueBrightness = 0;
  } 
  else if (sensorValue >= 350 && sensorValue < 371) {
    // Transition region from green to blue
    redBrightness = 0;
    greenBrightness = map(sensorValue, 350, 371, 255, 0); // Gradually decrease green
    blueBrightness = map(sensorValue, 350, 371, 0, 255); // Gradually increase blue
  } 
  else if (sensorValue >= 371) {
    // Fully blue for values 371 and above
    redBrightness = 0;
    greenBrightness = 0;
    blueBrightness = 255;
  }

  // Output the brightness levels to the RGB LED
  analogWrite(Red, redBrightness);
  analogWrite(Grn, greenBrightness);
  analogWrite(Blu, blueBrightness);

  // Adjust the servo position based on the sensor value:
  if (sensorValue < 320) {
    myservo.write(0);  // Move servo to 0° for red
  } 
  else if (sensorValue >= 320 && sensorValue < 371) {
    myservo.write(90);  // Move servo to 90° for green transition
  } 
  else {
    myservo.write(180);  // Move servo to 180° for blue
  }
  delay(300);
}
