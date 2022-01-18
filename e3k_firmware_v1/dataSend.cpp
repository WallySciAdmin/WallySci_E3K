/*
 * dataSend library
 * Contains functions to collect, encode and send data over serial and Bluetooth 
 * Author: Akshat Sahay
 */

/*
 * data is transmitted to the host program over serial/Bluetooth 
 * 
 * to minimise the size of samples, we use an encoding/decoding algorithm on both the device and host
 * the ESP32 uses a 12-bit ADC, so the smallest data structure for storage is a short int which wastes the 4 LSBs
 * 
 * to get around this, we store ADC values in a dataBuffer (array of bytes)
 * each ADC value is stored over 1.5 bytes in the dataBuffer, giving dataBuffer a max size of 9 bytes
 * without encoding, the 6 ADC values would use up 12 bytes, thus wasting 3 bytes during transmission
 */

#include "dataSend.h"

/* variables for encoding and storing ADC data */
unsigned char dataBuffer[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; // buffer for fitting in ADC data
unsigned char bufferLUT[7]  = {0, 2, 3, 5, 6, 8, 9};       // LUT storing bufferSize values
unsigned char bufferSize = 0; // number of bytes of the dataBuffer being transmitted 

unsigned char currentByte = 0;  // represents byte to be written to
unsigned char machineState = 1; // current state of state machine

/* getBufferSize: calculates the size of dataBuffer based on the number of channels using a LUT */
void getBufferSize() {
  // get buffer size based on channelSize
  bufferSize = bufferLUT[channelSize];
}

/* dataSend: sends data to the host based on configured communication channel, called in loop */
void dataSend() {
  // if state is on, send data
  if(state == 1) {
    // based on cMode, choose communication mode
    if(cMode == 0) dataSend_serial();
    else if(cMode == 1) dataSend_bluetooth();
    else if(cMode == 2) dataSend_WiFi();
    else dataSend_serial();
  }
}

/* ADC_getData: collects data from the ESP32's ADC, stores in channel[] */
void ADC_getData() {
  channel[0] = analogRead(32); 
  channel[1] = analogRead(39); 
  channel[2] = analogRead(36); 
  channel[3] = analogRead(33);   
  channel[4] = analogRead(35); 
  channel[5] = analogRead(34); 
}

/* dataEncode: encodes ADC data into dataBuffer for efficient packing of bits */
void dataEncode() {
  // get data from the ADC and the IMU
  ADC_getData();
  
  for(int i = 0; i < 6; i++) {
    // if the channel value is zero, don't send it 
    if(channelOn[i] == 0) {
      // skip the channel
      continue;
    }

    // case 1: no bits of val have been written and entire byte is empty
    // write first 8 bits, move on to the next byte and write 4 bits of the same val (state 3)
    if(machineState == 1) {
      dataBuffer[currentByte] |= (channel[i] >> 4) & (0b11111111);
      currentByte++; // move to next byte

      // same channel
      machineState = 3;
    }

    // case 2: no bits of val have been written, half byte is empty
    // write first 4 bits, move on to the next byte and write 8 bits of the same val (state 4)
    if(machineState == 2) {
      dataBuffer[currentByte] |= (channel[i] >> 8) & (0b00001111);
      currentByte++;

      // same channel
      machineState = 4;
    }

    // case 3: 8 bits of val have been written, entire byte is empty
    // write last 4 bits, stay on the same byte and write 4 bits of the next val (state 2)
    if(machineState == 3) {
      dataBuffer[currentByte] |= (channel[i] & 0b00001111) << 4;

      // new channel
      machineState = 2;
      continue;
    }

    // case 4: 4 bits of val have been written, entire byte is empty
    // write last 8 bits, move on to the next byte and write 8 bits of the next val (state 1)
    if(machineState == 4) {
      dataBuffer[currentByte] |= (channel[i] & 0b11111111) << 0;
      currentByte++;

      // new channel
      machineState = 1;
      continue;
    }
  }
}

/* dataSend_serial: sends data over the serial port to host connected via a USB cable */
void dataSend_serial() {
  dataEncode();    // encode data and store in dataBuffer 
  getBufferSize(); // calculate bufferSize based on active channels
  
  Serial.write((const byte*)&dataBuffer, bufferSize); // send data over the serial port as bytes
  
  // reset dataBuffer
  for(int j = 0; j < 15; j++) {
    dataBuffer[j] = 0;
  }

  currentByte = 0;  // reset currentByte
  machineState = 1; // reset state of dataEncode state machine
}

/* dataSend_bluetooth: sends data over Bluetooth to paired host device */
void dataSend_bluetooth() {
  dataEncode();    // encode data and store in dataBuffer 
  getBufferSize(); // calculate bufferSize based on active channels

  SerialBT.write((const byte*)&dataBuffer, bufferSize); // send data over Bluetooth as bytes

  // reset dataBuffer
  for(int j = 0; j < 15; j++) {
    dataBuffer[j] = 0;
  }

  currentByte = 0;  // reset currentByte
  machineState = 1; // reset state of dataEncode state machine
}

void dataSend_WiFi() {
  Serial.println("ERROR: Placeholder for Wi-Fi send");
}
