
#include "SCD30.h"
#include "SensorData.h"
#include "Aktuator.h"         // Motor control header
#include "WindowControl.h"    // Window control header for automatic parameters
#include "ManualControl.h"    // Manual control header for toggle switch

#define distanceSensorPin A0  // Analog pin for the distance sensor
bool overrideMode = false;
unsigned long overrideTargetTime = 0;
// Global SensorData instance
SensorData sensorData;

// Global variable to track the window state
bool isWindowOpen = false;

void setup() {
    Wire.begin();               // Initialize I2C communication
    Serial.begin(9600);         // Initialize Serial communication

    setupMotorPins();           // Setup motor pins and microswitches
    setupWindowControl();       // Initialize window control logic
    setupManualControl();       // Initialize manual control logic

    Serial.println("SCD30 Raw Data");
    scd30.initialize();         // Initialize SCD30 sensor
    Serial.println("SCD30 initialized.");
    Serial.println("System Ready. Type 'O' for open, 'C' for close, or toggle switch to control manually.");
}

void loop() {
    // Handle system toggle
    if(handleSystemToggle()==true){
        handleManualToggle(isWindowOpen);
        digitalWrite(systemLight,HIGH);
        digitalWrite(systemLightOff,LOW);
    } else {
        digitalWrite(systemLight,LOW);
        digitalWrite(systemLightOff,HIGH);

        // Handle manual toggle
        handleManualToggle(isWindowOpen);

        // Check override mode
        if (overrideMode) {
            // Check if target time reached
            if (millis() >= overrideTargetTime) {
                // Close the window
                if (isWindowOpen) {
                    closeWindow();
                    Serial.println("Window closed due to timer override.");
                }
                // Keep overrideMode active to prevent automatic control
            }
            // Bypass automatic control
        } else {
            // Automatic control logic
            static unsigned long lastUpdateTime = 0;  // For timing control
            const unsigned long updateInterval = 2000;  // Update interval in milliseconds

            if (millis() - lastUpdateTime >= updateInterval) {
                lastUpdateTime = millis();

                // Read and update sensor data
                readAndUpdateSensorData(sensorData);

                // Print sensor data to Serial
                printSensorData(sensorData);

                // Automatically decide window state
                decideWindowState(sensorData);
            }
        }
    }

    // Always check for UART commands
    handleSerialCommands();
}
