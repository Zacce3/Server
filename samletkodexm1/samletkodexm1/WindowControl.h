#ifndef WINDOWCONTROL_H
#define WINDOWCONTROL_H

#include <Arduino.h>
#include "SensorData.h"
#include "Aktuator.h"

// Thresholds for temperature, humidity, and CO2
extern double CO2_OPEN_THRESHOLD; 
extern double CO2_CLOSE_THRESHOLD; 
extern double TEMP_OPEN_THRESHOLD;
extern double TEMP_CLOSE_THRESHOLD; 
extern double HUMIDITY_OPEN_THRESHOLD; 
extern double HUMIDITY_CLOSE_THRESHOLD;
// Extern declaration for the global window state
extern bool isWindowOpen;
// Function declarations
void setupWindowControl();
void decideWindowState(const SensorData& sensorData);

#endif
