/*
  BasicRead.ino
  --------------
  Basic example to read distance from a TOF sensor using UART.

  This example:
  - Initializes the TOF sensor
  - Reads distance when valid data is received
  - Prints distance in centimeters
*/

#include <TOF_Sensor.h>

// Create TOF sensor object
// Change Serial if using a different UART
TOFSensor tof(Serial);

void setup() {
  // Start USB serial for debugging
  Serial.begin(115200);
  while (!Serial);   // Wait for Serial Monitor (Teensy)

  // Start TOF sensor UART
  tof.begin(115200);

  Serial.println("TOF Sensor - Basic Read Example");
}

void loop() {
  float distance;

  // Read distance from sensor
  if (tof.readDistance(distance)) {
    Serial.print("Distance: ");
    Serial.print(distance / 10.0);   // Convert mm to cm
    Serial.println(" cm");
  }
}
