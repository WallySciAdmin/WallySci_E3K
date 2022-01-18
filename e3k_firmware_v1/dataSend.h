/*
 * dataSend library 
 * Contains functions to collect, encode and send data over serial and Bluetooth 
 * Author: Akshat Sahay
 */

#ifndef DATA_SEND_H
#define DATA_SEND_H

#include "configDevice.h"

/* variables for encoding and storing ADC data */
extern unsigned char dataBuffer[9]; 
extern unsigned char bufferLUT[7];
extern unsigned char bufferSize;    

extern unsigned char currentByte;  
extern unsigned char machineState; 

/* functions for collecting and encoding ADC data */
void getBufferSize(); 
void ADC_getData();   
void dataEncode(); 

/* functions to send data over different modes, the other functions are called in dataSend */
void dataSend(); 
void dataSend_serial();
void dataSend_bluetooth();
void dataSend_WiFi();

#endif
