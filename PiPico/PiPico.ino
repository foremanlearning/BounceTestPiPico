#include <Arduino.h>

// Define constants for button pin and test duration
const int buttonPin = 0;  // GPIO pin for the button
const unsigned long testDuration = 1000;  // Test duration in milliseconds (1 second)

// Define variables for bounce counting and test control
int bounceCount = 0;  // Counter for the number of state changes (bounces)
int testNumber = 0;  // Counter to number the tests
bool testRunning = false;  // Flag to indicate if a test is running
unsigned long testStartTime = 0;  // Start time of the current test

void setup() {
  Serial.begin(115200);  // Initialize serial communication at 115200 baud rate
  pinMode(buttonPin, INPUT_PULLUP);  // Set button pin as input with internal pull-up resistor
}

void loop() {
  // Check if the button is pressed and a test is not running
  if (digitalRead(buttonPin) == LOW && !testRunning) {
    delay(10);  // Short delay for debounce
    // Confirm the button is still pressed after the debounce delay
    if (digitalRead(buttonPin) == LOW) {
      startTest();  // Start a new test
    }
  }

  // If a test is running
  if (testRunning) {
    unsigned long currentTime = millis();  // Get the current time in milliseconds
    // Check if the test duration has elapsed
    if (currentTime - testStartTime >= testDuration) {
      testRunning = false;  // Stop the test
      reportBounces();  // Report the bounce count
      delay(1000);  // Pause 1 second between tests
    } else {
      int reading = digitalRead(buttonPin);  // Read the current button state
      static int lastReading = HIGH;  // Variable to store the last button state
      // If the button state has changed
      if (reading != lastReading) {
        bounceCount++;  // Increment the bounce counter
        lastReading = reading;  // Update the last reading to the current state
      }
    }
  }
}

// Function to start a new test
void startTest() {
  bounceCount = 0;  // Reset the bounce counter
  testNumber++;  // Increment the test number
  testRunning = true;  // Set the test running flag to true
  testStartTime = millis();  // Record the start time of the test
  Serial.print("Starting test ");  // Print "Starting test " to the serial console
  Serial.println(testNumber);  // Print the test number to the serial console
}

// Function to report the bounce count
void reportBounces() {
  Serial.print("Test ");  // Print "Test " to the serial console
  Serial.print(testNumber);  // Print the test number to the serial console
  Serial.print(": ");  // Print ": " to the serial console
  Serial.print(bounceCount / 2);  // Print the number of bounces detected (divided by 2) to the serial console
  Serial.println(" bounces detected in 1 second.");  // Print " bounces detected in 1 second." to the serial console
}
