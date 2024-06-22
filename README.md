# Button Bounce Counter for Raspberry Pi Pico

## Purpose

This program is designed to run on a Raspberry Pi Pico using the Arduino IDE. Its primary purpose is to count the number of switch bounces during a 1-second button press on GPIO 0. Switch bounce is a common issue with mechanical switches where the contact bounces between open and closed states, causing multiple signals to be registered when the switch is pressed or released. This program helps in analyzing the bounce behavior of a button or switch.

## Methodology

1. **Button Press Detection**: The program monitors the state of a button connected to GPIO 0.
2. **Bounce Counting**: When the button is pressed, the program starts a 1-second test to count the number of state changes (bounces) detected.
3. **Debounce Handling**: A short delay is implemented to handle debounce and ensure accurate counting.
4. **Test Reporting**: After the 1-second duration, the program reports the number of bounces detected to the serial console.
5. **Sequential Tests**: The test can be repeated by pressing the button again, with each test being numbered sequentially.

## Quick Setup Instructions for Arduino IDE

### Step 1: Install the Arduino IDE
Download and install the Arduino IDE from the [official Arduino website](https://www.arduino.cc/en/software).

### Step 2: Add Raspberry Pi Pico Board Manager
1. Open the Arduino IDE.
2. Go to `File` -> `Preferences`.
3. In the `Additional Boards Manager URLs` field, add the following URL:
   ```
   https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
   ```
4. Click `OK`.

### Step 3: Install the Raspberry Pi Pico/RP2040 Board
1. Go to `Tools` -> `Board` -> `Boards Manager`.
2. Search for `Raspberry Pi Pico/RP2040`.
3. Install the package by `Earle F. Philhower, III`.

### Step 4: Select the Raspberry Pi Pico Board
1. Go to `Tools` -> `Board`.
2. Select `Raspberry Pi Pico`.

### Step 5: Connect the Raspberry Pi Pico
1. Connect your Raspberry Pi Pico to your computer via USB.
2. Select the appropriate port from `Tools` -> `Port`.

### Step 6: Upload the Program
1. Copy the provided program code into the Arduino IDE.
2. Click the `Upload` button to compile and upload the code to your Raspberry Pi Pico.

## Program Code

```cpp
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
    delay(0);  // Short delay for debounce
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
```

### Notes
- Ensure the button is properly connected to GPIO 0 and ground.
- The internal pull-up resistor is used to maintain a stable high state when the button is not pressed.
- Adjust the debounce delay if necessary to suit your specific button characteristics.

By following these instructions, you will be able to set up and run the button bounce counter on your Raspberry Pi Pico using the Arduino IDE.