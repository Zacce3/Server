#include "Aktuator.h"
#include "ManualControl.h"

// Global variable to track window state
extern bool isWindowOpen;

void setupMotorPins() {
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENA, OUTPUT);

    pinMode(openWindowSwitch, INPUT_PULLUP);
    pinMode(closeWindowSwitch, INPUT_PULLUP);
}

void openWindow() {
    Serial.println("Opening window...");
    digitalWrite(IN1, LOW); // Reverse the motor direction
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, 255); // Full speed

    // Keep driving until the "fully opened" microswitch is triggered
    while (digitalRead(openWindowSwitch) == LOW) {
        handleThresholdUpdate();       // Check for threshold update commands
        handleWindowCommands();        // Check for window commands
        handleManualToggle(isWindowOpen); // Check for manual toggle
        delay(10); // Small delay to prevent CPU overuse
    }

    stopMotor(); // Stop the motor when fully opened
    isWindowOpen = true; // Update state
    Serial.println("Window fully opened.");
}


void closeWindow() {
    Serial.println("Closing window...");
    digitalWrite(IN1, HIGH); // Forward the motor direction
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 255); // Full speed

    // Keep driving until the "fully closed" microswitch is triggered
    while (digitalRead(closeWindowSwitch) == LOW) {
        handleThresholdUpdate();       // Check for threshold update commands
        handleWindowCommands();        // Check for window commands
        handleManualToggle(isWindowOpen); // Check for manual toggle
        delay(10); // Small delay to prevent CPU overuse
    }

    stopMotor(); // Stop the motor when fully closed
    isWindowOpen = false; // Update state
    Serial.println("Window fully closed.");
}


void stopMotor() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);   // Stop motor
    Serial.println("Motor stopped.");
}

void handleWindowCommands() {
    if (Serial.available() > 0) {
        char command = Serial.read();
        if (command == 'O' || command == 'o') {
            openWindow();
        } else if (command == 'C' || command == 'c') {
            closeWindow();
        } else {
            Serial.println("Invalid command. Use 'O' for open, 'C' for close.");
        }
    }
}
