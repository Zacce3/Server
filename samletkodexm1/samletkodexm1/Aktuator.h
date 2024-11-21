#ifndef AKTUATOR_H
#define AKTUATOR_H

#include <Arduino.h>

// Pin definitions for motor control
#define IN1 8   // Connect IN1 on L298 to Arduino pin 8
#define IN2 9   // Connect IN2 on L298 to Arduino pin 9
#define ENA 10  // Connect ENA on L298 to Arduino pin 10 (PWM-capable)

// Microswitch pins
// Microswitch pins
#define openWindowSwitch 12   // Previously closeWindowSwitch
#define closeWindowSwitch 11  // Previously openWindowSwitch


// Function declarations
void setupMotorPins();
void openWindow();
void closeWindow();
void stopMotor();
void handleWindowCommands(); // Function to process UART motor commands
void handleSerialCommands();


#endif
