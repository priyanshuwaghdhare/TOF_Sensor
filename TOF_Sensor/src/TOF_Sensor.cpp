#include "TOF_Sensor.h"

// TOF protocol constants
#define TOF_FRAME_HEADER   0x57
#define TOF_FUNCTION_MARK  0x00

/*
  Frame format (16 bytes):
  Byte 0   : Frame header (0x57)
  Byte 1   : Function mark (0x00)
  Byte 8-10: Distance data
  Byte 15  : Checksum (sum of bytes 0–14)
*/

TOFSensor::TOFSensor(HardwareSerial &serial) {
  tofSerial = &serial;
  index = 0;
  checksum = 0;

  // Initialize filter variables
  filterIndex = 0;
  filterFilled = false;
}

void TOFSensor::begin(uint32_t baud) {
  // Start UART communication with the sensor
  tofSerial->begin(baud);
}

bool TOFSensor::readDistance(float &distance) {

  // Read all available bytes from the UART buffer
  while (tofSerial->available()) {
    uint8_t byteData = tofSerial->read();

    // Wait for frame header
    if (index == 0 && byteData != TOF_FRAME_HEADER) {
      continue;
    }

    // Store byte into receive buffer
    rx_buf[index++] = byteData;

    // Once full frame is received
    if (index >= 16) {

      // Calculate checksum
      checksum = 0;
      for (int i = 0; i < 15; i++) {
        checksum += rx_buf[i];
      }

      // Validate frame
      if (rx_buf[0] == TOF_FRAME_HEADER &&
          rx_buf[1] == TOF_FUNCTION_MARK &&
          checksum == rx_buf[15]) {

        // Extract distance bytes
        uint32_t rawDistance =
          ((uint32_t)rx_buf[10] << 24) |
          ((uint32_t)rx_buf[9]  << 16) |
          ((uint32_t)rx_buf[8]  << 8);

        // Convert raw value to distance
        float dist = (float)((long)rawDistance) / 256.0;

        // Apply moving average filter
        distance = applyFilter(dist);

        // Reset buffer index
        index = 0;
        return true;
      }

      // Reset buffer if frame is invalid
      index = 0;
    }
  }

  // No valid data received
  return false;
}

float TOFSensor::applyFilter(float newValue) {

  // Store new value in circular buffer
  filterBuffer[filterIndex++] = newValue;

  if (filterIndex >= FILTER_SIZE) {
    filterIndex = 0;
    filterFilled = true;
  }

  // Calculate average
  float sum = 0;
  int count = filterFilled ? FILTER_SIZE : filterIndex;

  for (int i = 0; i < count; i++) {
    sum += filterBuffer[i];
  }

  return sum / count;
}
