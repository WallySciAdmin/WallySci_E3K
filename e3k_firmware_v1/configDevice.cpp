/*
 * configDevice library
 * Contains functions to configure device from serial/Bluetooth data
 * Author: Akshat Sahay
 */

/*
 * device configuration is done using configByte received from our Python API
 * configByte is a 16-bit number with different bits mapped to different fields 
 * 
 * bit 15(state)       : start/stop bit, configures data stream to be ON or OFF
 * bits 14-13(cMode)   : communication mode bits, configure communication channel (UART, Bluetooth)
 * bits 12-11(dataRate): transmission rate bits, configure speed of transmission of packets (10Hz, 100Hz, 500Hz, 1000Hz)
 * bits 10-5(channel)  : channel bits, configure which ADC channels are currently being transmitted 
 * 
 * configuration occurs when data is available on a communication channel 
 * configByte can be received by either UART or Bluetooth
 */

#include "configDevice.h"

unsigned short int configByte = 0; // 16 bit number recieved from host program via serial

/* configuration variables */
byte state = 0; // default is OFF state
unsigned char cMode = 0; // default is serial
unsigned short int dataRate = 100; // default is 100Hz

unsigned short int channel[6] = {0, 0, 0, 0, 0, 0}; // default is no channels active
unsigned char channelOn[6] = {0, 0, 0, 0, 0, 0};    // default is no channels active
unsigned char channelSize = 0; // default is no channels active

/* setDataRate: sets transmission speed using bits 12-11 of configByte */
void setDataRate() {
  // set dataRate using the assigned configByte bits
  if(((configByte & (1 << 11 | 1 << 12)) >> 11) == 0)      dataRate = 10; 
  else if(((configByte & (1 << 11 | 1 << 12)) >> 11) == 1) dataRate = 100; 
  else if(((configByte & (1 << 11 | 1 << 12)) >> 11) == 2) dataRate = 500; 
  else if(((configByte & (1 << 11 | 1 << 12)) >> 11) == 3) dataRate = 1000; 
  else dataRate = 100; // default in error is 100Hz
}

/* setChannel: sets active and inactive channels using bits 10-5 of configByte */
void setChannel() {
  channelSize = 0; // reset channel size for new configuration
  
  if(((configByte & (1 << 10)) >> 10) == 1) {
    // channel 0 is active
    channelOn[0] = 1;
    channelSize++;
  }
  else {
    // channel 0 is inactive
    channel[0] = 0;
    channelOn[0] = 0;
  }
  if(((configByte & (1 << 9)) >> 9) == 1) {
    // channel 1 is active
    channelOn[1] = 1;
    channelSize++;
  }
  else {
    // channel 1 is inactive
    channel[1] = 0;
    channelOn[1] = 0;
  }
  if(((configByte & (1 << 8)) >> 8) == 1) {
    // channel 2 is active
    channelOn[2] = 1;
    channelSize++;
  }
  else {
    // channel 2 is inactive
    channel[2] = 0;
    channelOn[2] = 0;
  }
  if(((configByte & (1 << 7)) >> 7) == 1) {
    // channel 3 is active
    channelOn[3] = 1;
    channelSize++;
  }
  else {
    // channel 3 is inactive
    channel[3] = 0;
    channelOn[3] = 0;
  }
  if(((configByte & (1 << 6)) >> 6) == 1) {
    // channel 4 is active
    channelOn[4] = 1;
    channelSize++;
  }
  else {
    // channel 4 is inactive
    channel[4] = 0;
    channelOn[4] = 0;
  }
  if(((configByte & (1 << 5)) >> 5) == 1) {
    // channel 5 is active
    channelOn[5] = 1;
    channelSize++;
  }
  else {
    // channel 5 is inactive
    channel[5] = 0;
    channelOn[5] = 0;
  }
}

/* configDevice: checks if configByte is available, uses configByte to configure device, called in loop */
void configDevice() {
  // check if data is available on serial
  if(Serial.available()) {
    configByte = Serial.parseInt(); // receive configByte
    
    state = (configByte & (1 << 15)) >> 15; // set state, check first bit for configuration
    cMode = (configByte & (1 << 13 | 1 << 14)) >> 13; // set commMode, check next 2 bits for configuration 
    setDataRate(); // set dataRate function, check next 2 bits for configuration
    setChannel();  // set channel function, check the next 7 bits (6 -> ADC, 1 -> IMU) for configuration
  } 

  // check if data is available on the BluetoothSerial 
  if(SerialBT.available()) {
    configByte = SerialBT.parseInt(); // receive configByte
    
    state = (configByte & (1 << 15)) >> 15; // set state, check first bit for configuration
    cMode = (configByte & (1 << 13 | 1 << 14)) >> 13; // set commMode, check next 2 bits for configuration 
    setDataRate(); // set dataRate function, check next 2 bits for configuration
    setChannel();  // set channel function, check the next 7 bits (6 -> ADC, 1 -> IMU) for configuration 
  }
}
