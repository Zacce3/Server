#include "SensorData.h"
#include "WindowControl.h"
// Pin for the distance sensor
const int distanceSensorPin = A0;

void readAndUpdateSensorData(SensorData &data) {
    float result[3] = {0};

    // Use the default SCD30 object from the library
    if (scd30.isAvailable()) {
        scd30.getCarbonDioxideConcentration(result);
        data.co2 = result[0];
        data.temperature = result[1];
        data.humidity = result[2];
    } else {
        data.co2 = -1;
        data.temperature = -1;
        data.humidity = -1;
    }

    int sensorValue = analogRead(distanceSensorPin);
    float voltage = sensorValue * (5.0 / 1023.0);
    if (voltage >= 0.4 && voltage <= 2.8) {
        data.distance = 30 - (voltage - 0.4) * (30 - 4) / (2.8 - 0.4);
    } else if (voltage > 2.8) {
        data.distance = 4;
    } else {
        data.distance = 30;
    }
    
    data.windowOpen = (data.distance > 10);
}

void printSensorData(const SensorData &data) {
    Serial.print("CO2: ");
    Serial.print(data.co2);
    Serial.print(" ppm, ");
    Serial.print("Temperature: ");
    Serial.print(data.temperature);
    Serial.print(" °C, ");
    Serial.print("Humidity: ");
    Serial.print(data.humidity);
    Serial.print(" %, ");
    Serial.print("Distance: ");
    Serial.print(data.distance);
    Serial.print(" cm, ");
    Serial.print("Window: ");
    Serial.println(data.windowOpen ? "Open" : "Closed");

    // Send current threshold values
    Serial.print("Thresholds: ");
    Serial.print(CO2_OPEN_THRESHOLD); Serial.print(", ");
    Serial.print(CO2_CLOSE_THRESHOLD); Serial.print(", ");
    Serial.print(TEMP_OPEN_THRESHOLD); Serial.print(", ");
    Serial.print(TEMP_CLOSE_THRESHOLD); Serial.print(", ");
    Serial.print(HUMIDITY_OPEN_THRESHOLD); Serial.print(", ");
    Serial.println(HUMIDITY_CLOSE_THRESHOLD);
}

