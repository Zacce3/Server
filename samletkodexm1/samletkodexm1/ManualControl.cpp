#include "ManualControl.h"
#include "Aktuator.h"
#include"WindowControl.h"

// Global variable to track window state
extern bool isWindowOpen;


void setupManualControl() {
    pinMode(manualTogglePin, INPUT); // Active-high input
    pinMode(systemTogglePinOn, INPUT);// Active-high input
    pinMode(systemTogglePinOff, INPUT);// Active-high input
    
    Serial.println("Manual Control Initialized.");
}

void handleManualToggle(bool &isWindowOpen) {
    // Check the toggle switch state
    if (digitalRead(manualTogglePin) == HIGH) { // Switch is toggled ON
        if (isWindowOpen) {
            Serial.println("Manual toggle: Closing window...");
            closeWindow(); // Centralized function updates state
        } else {
            Serial.println("Manual toggle: Opening window...");
            openWindow(); // Centralized function updates state
        }

        // Wait for the toggle switch to be released (debounce)
        while (digitalRead(manualTogglePin) == HIGH) {
            delay(50); // Debounce delay
        }
    }
}
bool handleSystemToggle(){
  
  if(digitalRead(systemTogglePinOn)==HIGH){
    return false;
  }
  return true;
}
// In your Arduino code
void handleThresholdUpdate() {
    static String inputString = ""; // A String to hold incoming data
    while (Serial.available() > 0) {
        char inChar = (char)Serial.read();  // Read a character
        inputString += inChar;              // Append to inputString
        if (inChar == '\n') {               // If the incoming character is a newline
            inputString.trim();             // Remove leading and trailing whitespace
            if (inputString.startsWith("T,")) {
                processThresholdCommand(inputString);
            }
            inputString = ""; // Clear the string for the next command
        }
    }
}

void processThresholdCommand(String input) {
    // Remove the 'T,' prefix
    String commandData = input.substring(2);

    // Split the commandData by commas
    double values[6];
    int index = 0;
    int start = 0;

    while (index < 6 && start < commandData.length()) {
        int commaIndex = commandData.indexOf(',', start);
        String valueStr;

        if (commaIndex != -1) {
            valueStr = commandData.substring(start, commaIndex);
            start = commaIndex + 1;
        } else {
            valueStr = commandData.substring(start);
            start = commandData.length();
        }

        values[index] = valueStr.toFloat();
        index++;
    }

    // Update thresholds if we have all values
    if (index == 6) {
        CO2_OPEN_THRESHOLD = values[0];
        CO2_CLOSE_THRESHOLD = values[1];
        TEMP_OPEN_THRESHOLD = values[2];
        TEMP_CLOSE_THRESHOLD = values[3];
        HUMIDITY_OPEN_THRESHOLD = values[4];
        HUMIDITY_CLOSE_THRESHOLD = values[5];

        Serial.println("Thresholds updated:");
        Serial.print("CO2_OPEN_THRESHOLD: "); Serial.println(CO2_OPEN_THRESHOLD);
        Serial.print("CO2_CLOSE_THRESHOLD: "); Serial.println(CO2_CLOSE_THRESHOLD);
        Serial.print("TEMP_OPEN_THRESHOLD: "); Serial.println(TEMP_OPEN_THRESHOLD);
        Serial.print("TEMP_CLOSE_THRESHOLD: "); Serial.println(TEMP_CLOSE_THRESHOLD);
        Serial.print("HUMIDITY_OPEN_THRESHOLD: "); Serial.println(HUMIDITY_OPEN_THRESHOLD);
        Serial.print("HUMIDITY_CLOSE_THRESHOLD: "); Serial.println(HUMIDITY_CLOSE_THRESHOLD);

        // Send back the updated thresholds immediately
        Serial.print("Thresholds: ");
        Serial.print(CO2_OPEN_THRESHOLD); Serial.print(", ");
        Serial.print(CO2_CLOSE_THRESHOLD); Serial.print(", ");
        Serial.print(TEMP_OPEN_THRESHOLD); Serial.print(", ");
        Serial.print(TEMP_CLOSE_THRESHOLD); Serial.print(", ");
        Serial.print(HUMIDITY_OPEN_THRESHOLD); Serial.print(", ");
        Serial.println(HUMIDITY_CLOSE_THRESHOLD);
    } else {
        Serial.println("Error: Incorrect number of threshold values received.");
    }
}
