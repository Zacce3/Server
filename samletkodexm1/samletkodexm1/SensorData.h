#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <Arduino.h>
#include "SCD30.h"

// Struct to store sensor data
struct SensorData {
    float co2;
    float temperature;
    float humidity;
    float distance;
    bool windowOpen;
};

// Function declarations
void readAndUpdateSensorData(SensorData &data);
void printSensorData(const SensorData &data);

#endif
