
#include "SCD30.h"
#include "SensorData.h"
#include "Aktuator.h"         // Motor control header
#include "WindowControl.h"    // Window control header for automatic parameters
#include "ManualControl.h"    // Manual control header for toggle switch

#define distanceSensorPin A0  // Analog pin for the distance sensor

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
    static unsigned long lastUpdateTime = 0;  // For timing control
    const unsigned long updateInterval = 2000;  // Update interval in milliseconds

    // Pass the isWindowOpen variable to handleManualToggle
    handleManualToggle(isWindowOpen);

    // Automatic control logic
    if (millis() - lastUpdateTime >= updateInterval) {
        lastUpdateTime = millis();

        // Read and update sensor data
        readAndUpdateSensorData(sensorData);

        // Print sensor data to Serial
        printSensorData(sensorData);

        // Automatically decide window state
        decideWindowState(sensorData);
    }

    // Always check for UART commands
    //handleWindowCommands();
    handleSerialCommands();
    // Check for threshold update commands
    //andleThresholdUpdate();
}

