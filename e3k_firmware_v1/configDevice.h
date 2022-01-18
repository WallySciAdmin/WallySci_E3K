/*
 * configDevice library
 * Contains functions to configure device from serial/Bluetooth data
 * Author: Akshat Sahay
 */

#ifndef CONFIG_DEVICE_H
#define CONFIG_DEVICE_H

#include "bluetoothSetup.h"

/* variables for controlling device configuration */
extern unsigned short int configByte; 

extern byte state;
extern unsigned char cMode; 
extern unsigned short int dataRate;

/* variables for controlling channels and storing raw data */ 
extern unsigned short int channel[6]; 
extern unsigned char channelOn[6];    
extern unsigned char channelSize;

/* functions for configuring device over communication modes */
void setDataRate(); 
void setChannel(); 
void configDevice();

#endif
