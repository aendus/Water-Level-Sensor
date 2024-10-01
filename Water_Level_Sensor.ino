#include <ESP8266WiFi.h>

// Define pin numbers
const int ledPin = 5;          // GPIO5 (D1)
const int buttonPin = 4;       // GPIO4 (D2)
const int magnetSensorPin = A0; // A0 corresponds to GPIO2 (D4)

// Variables to hold the state of the inputs
bool buttonPressed = false;
bool ledBlinking = false;
unsigned long previousMillis = 0;
const long interval = 500; // Interval for blinking LED (500ms on/off)

// Counter for consecutive readings in the range
int consecutiveCount = 0;

void setup() {
  // Set up the LED pin as an output
  pinMode(ledPin, OUTPUT);
  
  // Set up the button and magnet sensor pins as inputs
  pinMode(buttonPin, INPUT_PULLUP);  // Use internal pull-up resistor
  pinMode(magnetSensorPin, INPUT);    // Set A0 (GPIO2) as input
  
  // Initialize the LED state to OFF
  digitalWrite(ledPin, LOW);

  // Initialize Serial communication
  Serial.begin(115200); // Start serial communication at 115200 baud rate

  // Disable Wi-Fi
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);
  WiFi.forceSleepWake();
}

void loop() {
  // Read the state of the magnet sensor and button
  int magnetValue = analogRead(magnetSensorPin); // Read analog value from A0 (GPIO2)
  buttonPressed = digitalRead(buttonPin) == LOW;   // Assuming LOW when pressed

  // Print the sensor readings to the Serial Monitor
  Serial.print("Magnet Sensor Value: ");
  Serial.println(magnetValue);
  
  // Check if the magnet sensor value is in the range 4 to 8
  if (magnetValue >= 5 && magnetValue <= 5) {
    consecutiveCount++; // Increment the counter if in range
  } else {
    consecutiveCount = 0; // Reset the counter if out of range
  }

  // Check if we have 3 or more consecutive readings in range
  if (consecutiveCount >= 12) {
    // Start blinking LED if not already blinking
    if (!ledBlinking) {
      ledBlinking = true;
      previousMillis = millis(); // Reset timer
    }
  }

  // Stop blinking if button is pressed
  if (buttonPressed) {
    ledBlinking = false; // Stop blinking
    digitalWrite(ledPin, LOW); // Ensure LED is off
  }

  // Blink the LED if ledBlinking is true
  if (ledBlinking) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      // Toggle LED state
      digitalWrite(ledPin, !digitalRead(ledPin));
    }
  }

  // Ensure LED is off if not blinking and button is pressed
  if (!ledBlinking && buttonPressed) {
    digitalWrite(ledPin, LOW);
  }
}
