/*
 * bluetoothSetup library
 * Library contains functions to initialize Bluetooth on E3K with a unique name 
 * Author: Varan Gupta
 */

/*
 * Bluetooth is initialised on the device on boot time using the BluetoothSerial library 
 * each device gets a unique MAC address using getDeviceAddress
 * 
 * connection to the device happens automatically using the pyBluez library from the host program
 */

#include "bluetoothSetup.h"

BluetoothSerial SerialBT;
int addr = 0;
int val = 0;

void getDeviceAddress() {
  const uint8_t* point = esp_bt_dev_get_address();
  EEPROM.write(0, 1); // storing MAC in EEPROM

  String str;
  for (int i = 0; i < 6; i++) {
    EEPROM.write(i + 1, point[i]);
    str += String(point[i], HEX);
    if (i < 5) {
      str += ":";
    }
  }
  str.toUpperCase();
  Serial.print("Following MAC addr. stored in EEPROM:");
  Serial.println(str);
  EEPROM.commit();

  String ble_name = "E3K";
  for (int i = 3; i < 6; i++) {
    ble_name += "_";
    ble_name += String(point[i], HEX);
  }
  ble_name.toUpperCase();
  SerialBT.begin(ble_name); //Bluetooth device name
  Serial.print("Device bluetooth name:");
  Serial.println(ble_name);
}

void initBT() {
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("failed to initialise EEPROM"); delay(1000000);
  }

  byte ble_state = EEPROM.read(0); //check if name already stored
  if ( ble_state == 0x1) {
    int i;
    String ble_name = "E3K";
    for (int i = 3; i < 6; i++) {
      ble_name += "_";
      ble_name += String(EEPROM.read(i + 1), HEX); //read MAC addr from EEPROM
    }
    
    ble_name.toUpperCase();
    SerialBT.begin(ble_name); //Bluetooth device name
    Serial.print("Device bluetooth name:");
    Serial.println(ble_name);
    Serial.println("The device started, now you can pair it with bluetooth!");
  }
  else {
    //    Serial.println("Name not set");
    SerialBT.begin("E3K_test"); // bluetooth required to be started before reading MAC addr
    getDeviceAddress();
    Serial.println("The device started, now you can pair it with bluetooth!");
  }
}
