/*
 * bluetoothSetup library
 * Library contains functions to initialize Bluetooth on E3K with a unique name 
 * Author: Varan Gupta
 */

#ifndef BT_SETUP_H
#define BT_SETUP_H

#include "BluetoothSerial.h"
#include "esp_bt_device.h"
#include "EEPROM.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define EEPROM_SIZE 7

extern BluetoothSerial SerialBT;

/* integer declarations for address and value */
extern int addr;
extern int val;

/* functions for generating unique MAC address */
void getDeviceAddress();
void initBT();

#endif
