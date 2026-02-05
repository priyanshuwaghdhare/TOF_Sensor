#ifndef TOF_SENSOR_H
#define TOF_SENSOR_H

#include <Arduino.h>

/*
  TOFSensor
  ----------
  A UART-based Time-of-Flight distance sensor driver.

  Features:
  - Uses HardwareSerial (Teensy, ESP32, Arduino Mega compatible)
  - Validates incoming data frames
  - Built-in moving average filter for stable readings
*/

class TOFSensor {
public:
  /*
    Constructor
    @param serial : HardwareSerial object (Serial1, Serial2, Serial5, etc.)
  */
  TOFSensor(HardwareSerial &serial);

  /*
    Initialize the UART communication
    @param baud : Baud rate of the TOF sensor (e.g. 115200)
  */
  void begin(uint32_t baud);

  /*
    Read distance from the sensor
    @param distance : Output distance value (filtered)
    @return true if a valid frame was received
  */
  bool readDistance(float &distance);

private:
  // Pointer to the serial port used by the sensor
  HardwareSerial *tofSerial;

  // Receive buffer for one full data frame (16 bytes)
  uint8_t rx_buf[16];

  // Index to track received bytes
  uint8_t index;

  // Checksum value for frame validation
  uint8_t checksum;

  // ---------------- FILTER VARIABLES ----------------
  static const int FILTER_SIZE = 10;   // Number of samples for averaging
  float filterBuffer[FILTER_SIZE];     // Circular buffer
  int filterIndex;                     // Current index
  bool filterFilled;                   // Indicates buffer is full

  /*
    Apply moving average filter
    @param newValue : New distance sample
    @return Filtered distance value
  */
  float applyFilter(float newValue);
};

#endif
