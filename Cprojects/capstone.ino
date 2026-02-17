/* #include <Wire.h>
#include "Adafruit_AS726x.h"
#include <LiquidCrystal.h>  // Standard LCD library for parallel communication

// Create the object for AS726x sensor
Adafruit_AS726x ams;

// Buffer to hold raw values
uint16_t sensorValues[AS726x_NUM_CHANNELS];

// Counters for colors
int orangeCounter = 0;
int greenCounter = 0;
int yellowCounter = 0;
int blueCounter = 0;  // Changed redCounter to blueCounter

// Threshold for significant change (to avoid counting small fluctuations)
const uint16_t significantChangeThreshold = 350;

// Variables to store the last values for each color to detect significant changes
uint16_t lastOrange = 0;
uint16_t lastGreen = 0;
uint16_t lastYellow = 0;
uint16_t lastBlue = 0;  // Changed lastRed to lastBlue

// Initialize the LCD (Adjust the pins based on your setup)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // RS, E, D4, D5, D6, D7 (pins for parallel LCD)

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Initialize digital pin LED_BUILTIN as an output
  pinMode(LED_BUILTIN, OUTPUT);

  // Begin and make sure we can talk to the sensor
  if (!ams.begin()) {
    Serial.println("Could not connect to sensor! Please check your wiring.");
    while (1);
  }

  // Initialize the LCD
  lcd.begin(16, 2);  // 16 columns and 2 rows LCD
  lcd.clear();
  lcd.print("Color Counter");

  delay(1000); // Wait for a second before starting measurements
}

void loop() {
  // Start the measurement
  ams.startMeasurement();

  // Wait until data is available
  bool rdy = false;
  while (!rdy) {
    delay(5);
    rdy = ams.dataReady();
  }

  // Read the raw sensor values
  ams.readRawValues(sensorValues);

  // Detect significant changes and increment counters once
  bool updated = false;

  // Check if the values have changed significantly before incrementing counters
  if (abs(sensorValues[AS726x_ORANGE] - lastOrange) > significantChangeThreshold) {
    lastOrange = sensorValues[AS726x_ORANGE];
    orangeCounter++;
    updated = true;
  }

  if (abs(sensorValues[AS726x_GREEN] - lastGreen) > significantChangeThreshold) {
    lastGreen = sensorValues[AS726x_GREEN];
    greenCounter++;
    updated = true;
  }

  if (abs(sensorValues[AS726x_YELLOW] - lastYellow) > significantChangeThreshold) {
    lastYellow = sensorValues[AS726x_YELLOW];
    yellowCounter++;
    updated = true;
  }

  if (abs(sensorValues[AS726x_BLUE] - lastBlue) > significantChangeThreshold) {  // Changed red to blue
    lastBlue = sensorValues[AS726x_BLUE];  // Changed lastRed to lastBlue
    blueCounter++;  // Changed redCounter to blueCounter
    updated = true;
  }

  // Only update the LCD if there was a change
  if (updated) {
    lcd.clear();
    lcd.setCursor(0, 0);  // Set cursor to the first row
    lcd.print("O: ");
    lcd.print(orangeCounter);
    lcd.print(" G: ");
    lcd.print(greenCounter);

    lcd.setCursor(0, 1);  // Set cursor to the second row
    lcd.print("Y: ");
    lcd.print(yellowCounter);
    lcd.print(" B: ");  // Changed R to B for Blue
    lcd.print(blueCounter);  // Display Blue counter

    delay(500); // Delay to make the LCD readable
  }

  // Optionally, print raw data to the serial monitor for debugging
  Serial.print(" Blue: "); Serial.print(sensorValues[AS726x_BLUE]);
  Serial.print(" Green: "); Serial.print(sensorValues[AS726x_GREEN]);
  Serial.print(" Yellow: "); Serial.print(sensorValues[AS726x_YELLOW]);
  Serial.print(" Orange: "); Serial.print(sensorValues[AS726x_ORANGE]);
  Serial.print(" Blue Count: "); Serial.print(blueCounter);  // Changed Red to Blue
  Serial.print(" Orange Count: "); Serial.print(orangeCounter);
  Serial.print(" Green Count: "); Serial.print(greenCounter);
  Serial.print(" Yellow Count: "); Serial.print(yellowCounter);
  Serial.println();
}
#include <Servo.h> // Include the Servo library

Servo servo1; // Create a Servo object for the first motor
Servo servo2; // Create a Servo object for the second motor

void setup() {
  // Setup for Servo 1
  servo1.attach(9); // Attach servo1 to pin 9
  servo1.write(90); // Set initial position to 90 degrees
  delay(2000); // Wait 2 seconds to stabilize

  // Setup for Servo 2
  servo2.attach(10); // Attach servo2 to pin 10
  servo2.write(0); // Set initial position to 0 degrees
  delay(1000); // Wait 1 second to stabilize
}

void loop() {
  // Behavior for Servo 1
  for (int pos = 90; pos <= 180; pos++) { // Move from 90 to 180 degrees
    servo1.write(pos);
    delay(9); // Slightly increased delay for 2% slower movement
  }
  delay(2000); // Wait 2 seconds at 180 degrees

  for (int pos = 180; pos >= 148; pos--) { // Move back to 135 degrees
    servo1.write(pos);
    delay(9);
  }
  delay(5000); // Stop for 5 seconds at 135 degrees

  for (int pos = 135; pos >= 90; pos--) { // Move back to 90 degrees
    servo1.write(pos);
    delay(9);
  }
  delay(2000); // Wait 2 seconds at 90 degrees

  // Behavior for Servo 2
  servo2.write(30); // Move to 30 degrees
  delay(1000); // Wait 1 second

  servo2.write(60); // Move to 60 degrees
  delay(1000); // Wait 1 second

  servo2.write(90); // Move to 90 degrees
  delay(1000); // Wait 1 second

  servo2.write(0); // Move back to 0 degrees
  delay(1000); // Wait 1 second
}
*/
#include <Wire.h>
#include "Adafruit_AS726x.h"
#include <LiquidCrystal.h> // Standard LCD library for parallel communication
#include <Servo.h>         // Include the Servo library

// Create objects for sensors and servo motors
Adafruit_AS726x ams;
Servo servo1; // Create a Servo object for the first motor
Servo servo2; // Create a Servo object for the second motor

// Buffer to hold raw values
uint16_t sensorValues[AS726x_NUM_CHANNELS];

// Counters for colors
int orangeCounter = 0;
int greenCounter = 0;
int yellowCounter = 0;
int blueCounter = 0;

// Threshold for significant change (to avoid counting small fluctuations)
const uint16_t significantChangeThreshold = 50;

// Variables to store the last values for each color to detect significant changes
uint16_t lastOrange = 0;
uint16_t lastGreen = 0;
uint16_t lastYellow = 0;
uint16_t lastBlue = 0;

// Initialize the LCD (Adjust the pins based on your setup)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // RS, E, D4, D5, D6, D7 (pins for parallel LCD)

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Initialize digital pin LED_BUILTIN as an output
  pinMode(LED_BUILTIN, OUTPUT);

  // Begin and make sure we can talk to the sensor
  if (!ams.begin()) {
    Serial.println("Could not connect to sensor! Please check your wiring.");
    while (1);
  }

  // Initialize the LCD
  lcd.begin(16, 2); // 16 columns and 2 rows LCD
  lcd.clear();
  lcd.print("Color Counter");

  // Setup for Servo 1
  servo1.attach(6); // Attach servo1 to pin 6
  servo1.write(90); // Set initial position to 90 degrees
  delay(2000); // Wait 2 seconds to stabilize

  // Setup for Servo 2
  servo2.attach(10); // Attach servo2 to pin 10
  servo2.write(0); // Set initial position to 0 degrees
  delay(1000); // Wait 1 second to stabilize

  //delay(1000); // Wait for a second before starting measurements
}

void loop() {
  // Handle Servo 1 Behavior


  // Start the measurement for Servo 2
  //ams.startMeasurement();

  // Wait until data is available
//  bool rdy = false;
  //while (!rdy) {
    //delay(5);
    //rdy = ams.dataReady();
  //}

  // Read the raw sensor values
  //ams.readRawValues(sensorValues);


ams.startMeasurement();
if (ams.dataReady()) { // Immediately check if data is ready
    ams.readRawValues(sensorValues);
}


  // Detect significant changes and increment counters once
  bool updated = false;

  if (abs(sensorValues[AS726x_ORANGE] - lastOrange) > significantChangeThreshold) {
    lastOrange = sensorValues[AS726x_ORANGE];
    orangeCounter++;
    updated = true;
  }

  if (abs(sensorValues[AS726x_GREEN] - lastGreen) > significantChangeThreshold) {
    lastGreen = sensorValues[AS726x_GREEN];
    greenCounter++;
    updated = true;
  }

  if (abs(sensorValues[AS726x_YELLOW] - lastYellow) > significantChangeThreshold) {
    lastYellow = sensorValues[AS726x_YELLOW];
    yellowCounter++;
    updated = true;
  }

  if (abs(sensorValues[AS726x_BLUE] - lastBlue) > significantChangeThreshold) {
    lastBlue = sensorValues[AS726x_BLUE];
    blueCounter++;
    updated = true;
  }

  // Move Servo 2 based on the most recent color counter
  if (updated) {
    int servo2Pos = 0; // Default to 0 (Orange)
    if (greenCounter > orangeCounter && greenCounter > yellowCounter && greenCounter > blueCounter) {
      servo2Pos = 30; // Green
    } else if (blueCounter > orangeCounter && blueCounter > yellowCounter && blueCounter > greenCounter) {
      servo2Pos = 60; // Blue
    } else if (yellowCounter > orangeCounter && yellowCounter > greenCounter && yellowCounter > blueCounter) {
      servo2Pos = 90; // Yellow
    }
    servo2.write(servo2Pos);

    // Update the LCD
    lcd.clear();
    lcd.setCursor(0, 0); // Set cursor to the first row
    lcd.print("O: ");
    lcd.print(orangeCounter);
    lcd.print(" G: ");
    lcd.print(greenCounter);

    lcd.setCursor(0, 1); // Set cursor to the second row
    lcd.print("Y: ");
    lcd.print(yellowCounter);
    lcd.print(" B: ");
    lcd.print(blueCounter);

    delay(500); // Delay to make the LCD readable
  }

  // Optionally, print raw data to the serial monitor for debugging
  Serial.print(" Blue: "); Serial.print(sensorValues[AS726x_BLUE]);
  Serial.print(" Green: "); Serial.print(sensorValues[AS726x_GREEN]);
  Serial.print(" Yellow: "); Serial.print(sensorValues[AS726x_YELLOW]);
  Serial.print(" Orange: "); Serial.print(sensorValues[AS726x_ORANGE]);
  Serial.print(" Blue Count: "); Serial.print(blueCounter);
  Serial.print(" Orange Count: "); Serial.print(orangeCounter);
  Serial.print(" Green Count: "); Serial.print(greenCounter);
  Serial.print(" Yellow Count: "); Serial.print(yellowCounter);
  Serial.println();
}
