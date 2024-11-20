#include "WindowControl.h"

double CO2_OPEN_THRESHOLD = 1000.0;    // ppm
double CO2_CLOSE_THRESHOLD = 800.0;    // ppm
double TEMP_OPEN_THRESHOLD = 27.0;     // °C
double TEMP_CLOSE_THRESHOLD = 23.0;    // °C
double HUMIDITY_OPEN_THRESHOLD = 70.0; // %
double HUMIDITY_CLOSE_THRESHOLD = 60.0; // %
// Function to initialize any dependencies (if needed)
void setupWindowControl() {
    // Currently, no additional setup is required for this module.
    Serial.println("Window Control Initialized.");
}

// Decide whether the window should open or close based on sensor data
void decideWindowState(const SensorData& sensorData) {
    // Validate sensor data
    if (sensorData.co2 == -1 || sensorData.temperature == -1 || sensorData.humidity == -1) {
        Serial.println("Invalid sensor readings. No action will be taken.");
        return; // Skip any action if readings are invalid
    }

    // Open window conditions
    bool shouldOpen = 
        (sensorData.co2 > CO2_OPEN_THRESHOLD) ||
        (sensorData.temperature > TEMP_OPEN_THRESHOLD) ||
        (sensorData.humidity > HUMIDITY_OPEN_THRESHOLD);

    // Close window conditions
    bool shouldClose = 
        (sensorData.co2 < CO2_CLOSE_THRESHOLD) &&
        (sensorData.temperature < TEMP_CLOSE_THRESHOLD) &&
        (sensorData.humidity < HUMIDITY_CLOSE_THRESHOLD);

    // Take action based on conditions and current state
    if (shouldOpen && !isWindowOpen) {
        openWindow();
        isWindowOpen = true;
        Serial.println("Action: Window Opened.");
    } else if (shouldClose && isWindowOpen) {
        closeWindow();
        isWindowOpen = false;
        Serial.println("Action: Window Closed.");
    } else {
        Serial.println("Action: No change to window state.");
    }
}
