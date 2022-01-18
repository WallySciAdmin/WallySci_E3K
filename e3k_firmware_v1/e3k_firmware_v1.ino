/*
 * E3K Firmware v1.0
 * Bio-sensor module using the ESP32 and analog sensors (ECG, EEG, EMG)
 * Current implementation has configuration and data transmission over serial and Bluetooth
 * Author: Akshat Sahay
 * Credit: Akshat Sahay, Varan Gupta, Nanditha Sreenivas 
 */

#include "dataSend.h"

unsigned long currentTime = 0; // temp variable for delay calculation
unsigned short int waitTime = 1000/dataRate; // delay time

/* setup: runs once, initialises serial and Bluetooth ports */
void setup() {
  Serial.begin(115200); // initialise serial monitor at 115200 baud rate
  initBT(); // initialise Bluetooth using name based on MAC address 
}

/* loop: runs forever, configures device and transmits data to host program */
void loop() {
  // delay using time-division multiplexing 
  if(millis() - currentTime > waitTime) {
    configDevice(); // run every loop, configure E3K if serial has an input waiting
    dataSend();     // send data from ADC depending on configuration

    waitTime = 1000/dataRate; // delay time based on speed setting
    currentTime = millis();   // reset current time to actually current time
  }
}
