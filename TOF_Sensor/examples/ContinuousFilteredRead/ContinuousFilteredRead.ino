/*
  ContinuousFilteredRead.ino
  --------------------------
  Advanced example demonstrating continuous distance monitoring
  using the built-in moving average filter.

  This example:
  - Continuously reads distance
  - Prints stable filtered output
  - Uses non-blocking sensor reads
*/

#include <TOF_Sensor.h>

// Create TOF sensor object
TOFSensor tof(Serial);

// Timing variables
unsigned long lastPrintTime = 0;
const unsigned long PRINT_INTERVAL = 200;  // ms

void setup() {
  Serial.begin(115200);
  while (!Serial);

  tof.begin(115200);

  Serial.println("TOF Sensor - Continuous Filtered Read");
}

void loop() {
  float distance;

  // Read distance (non-blocking)
  if (tof.readDistance(distance)) {

    // Print at fixed interval
    if (millis() - lastPrintTime >= PRINT_INTERVAL) {
      lastPrintTime = millis();

      Serial.print("Filtered Distance: ");
      Serial.print(distance / 10.0);   // Convert mm to cm
      Serial.println(" cm");
    }
  }
}
