#include <Wire.h>
#include <Adafruit_TCS34725.h>  // Include the TCS34725 library
#include <LiquidCrystal.h>      // Standard LCD library for parallel communication
#include <Servo.h>              // Include the Servo library for Servo motors

// Create objects for sensors and servo motors
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);  // Initialize TCS34725 sensor
Servo servo1; // Create a Servo object for the first motor
Servo servo2; // Create a Servo object for the second motor

// Baseline (calibration) values
float baselineRed = 0;
float baselineGreen = 0;
float baselineBlue = 0;

// Counters for colors
int redCounter = 0;
int greenCounter = 0;
int blueCounter = 0;
int yellowCounter = 0;

// Servo positions for each color
const int SERVO_POS_RED = 0;
const int SERVO_POS_YELLOW = 30;
const int SERVO_POS_GREEN = 60;
const int SERVO_POS_BLUE = 90;

// Last scan time
unsigned long lastScanTime = 0;
const unsigned long scanInterval = 5000; // 5 seconds

// Initialize the LCD (Adjust the pins based on your setup)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // RS, E, D4, D5, D6, D7 (pins for parallel LCD)

void calibrateSensor() {
  float tempRed, tempGreen, tempBlue;

  Serial.println("Calibrating sensor... Ensure the environment is dark or covered.");
  delay(2000); // Give time to set up the calibration environment
  
  // Take multiple readings to average the baseline
  int samples = 10;
  for (int i = 0; i < samples; i++) {
    tcs.getRGB(&tempRed, &tempGreen, &tempBlue);
    baselineRed += tempRed;
    baselineGreen += tempGreen;
    baselineBlue += tempBlue;
    delay(100); // Small delay between readings
  }
  
  // Calculate the average baseline values
  baselineRed /= samples;
  baselineGreen /= samples;
  baselineBlue /= samples;

  Serial.print("Calibration complete. Baseline values - ");
  Serial.print("Red: "); Serial.print(baselineRed);
  Serial.print(", Green: "); Serial.print(baselineGreen);
  Serial.print(", Blue: "); Serial.println(baselineBlue);
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Begin the TCS34725 sensor
  if (tcs.begin()) {
    Serial.println("Found TCS34725 sensor!");
  } else {
    Serial.println("No TCS34725 sensor found. Please check wiring.");
    while (1);  // Halt if sensor is not found
  }

  // Initialize the LCD
  lcd.begin(16, 2);  // 16 columns and 2 rows LCD
  lcd.clear();
  lcd.print("Color Counter");

  delay(1000); // Wait for a second before starting measurements

  // Perform sensor calibration
  calibrateSensor();

  // Setup for Servo 1
  servo1.attach(6); // Attach servo1 to pin 6
  servo1.write(90); // Set initial position to 90 degrees
  delay(2000); // Wait 2 seconds to stabilize

  // Setup for Servo 2
  servo2.attach(10); // Attach servo2 to pin 10
  servo2.write(0); // Set initial position to the far left (red position)
  delay(1000); // Wait 1 second to stabilize
}

void loop() {
  unsigned long currentTime = millis();

  // Check if it's time to scan
  if (currentTime - lastScanTime >= scanInterval) {
    lastScanTime = currentTime; // Update the last scan time

    // Start the color reading
    float rawRed, rawGreen, rawBlue;
    tcs.getRGB(&rawRed, &rawGreen, &rawBlue);  // Get raw RGB values from the sensor

    // Adjust the raw readings by subtracting the baseline
    float red = max(0, rawRed - baselineRed);
    float green = max(0, rawGreen - baselineGreen);
    float blue = max(0, rawBlue - baselineBlue);

    // Detect significant changes and increment counters
    bool updated = false;

    if (red > 10 && green < 3 && blue < 3) {
      redCounter++;
      servo2.write(SERVO_POS_RED); // Move servo to the far left
      updated = true;
    } else if (red > 3 && green > 3) {
      yellowCounter++;
      servo2.write(SERVO_POS_YELLOW); // Move servo to the second from left
      updated = true;
    } else if (green > 10 && red < 3 && blue < 3) {
      greenCounter++;
      servo2.write(SERVO_POS_GREEN); // Move servo to the third from left
      updated = true;
    } else if (blue > 10 && green < 3 && red < 3) {
      blueCounter++;
      servo2.write(SERVO_POS_BLUE); // Move servo to the far right
      updated = true;
    }

    // Update the LCD if there was a change
    if (updated) {
      lcd.clear();
      lcd.setCursor(0, 0);  // Set cursor to the first row
      lcd.print("R: ");
      lcd.print(redCounter);
      lcd.print(" G: ");
      lcd.print(greenCounter);

      lcd.setCursor(0, 1);  // Set cursor to the second row
      lcd.print("B: ");
      lcd.print(blueCounter);
      lcd.print(" Y: ");
      lcd.print(yellowCounter);

      delay(500); // Small delay to make the LCD readable
    }

    // Print adjusted values to the serial monitor
    Serial.print("Adjusted R: "); Serial.print(red);
    Serial.print(" G: "); Serial.print(green);
    Serial.print(" B: "); Serial.println(blue);
  }
}
