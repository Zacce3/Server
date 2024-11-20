#ifndef MANUALCONTROL_H
#define MANUALCONTROL_H

#include <Arduino.h>

// Pin definitions for manual control
#define manualTogglePin 2

// Function declarations
void setupManualControl();
void handleManualToggle(bool &isWindowOpen);
void handleThresholdUpdate();
void processThresholdCommand(String input);

#endif
