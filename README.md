# WallySci_E3K
E3K is your framework for an intuitive understanding of Bio-signals. Use the kit to explore and build your own Electromyography (EMG), Electrocardiography (ECG) and Electroencephalography (EEG) machine. E3K is your Bio-Sensing kit for a variety of project needs in Robotics, Biomechanics, Biosensing, Physiology, Quantitative Feature Extraction, Real-time data transfer and visualization, Multisensory Data Acquisition or AI frameworks.

This repository features firmware and software for your E3K and host computer respectively. 

## Installation
### Firmware 
The E3K firmware is written in the Arduino framework using the ESP32 MCU's custom libraries. To install it onto your E3K: connect it to your host computer, add the ESP32 Arduino Core through the boards managar inside the Arduino IDE, select the correct COM port and push the firmware on the device. 

### Software 
The E3K API written in Python allows users to add the E3K into their projects without having to deal directly with the hardware. To use, you first need to add the pySerial and pyBluez modules into your Python project. Then, simply include e3k_API.py in your Python project and look at the example.py script to understand how to use the API. 

If you want to utilise the Bluetooth capabilities of the E3K in your project, make sure your computer has a Bluetooth module connected (either an in-built one or an external USB one). 

## Overview
The E3K firmware and software work together to collect data from the attached EMG, EEG and ECG sensors and send it at high speeds from the device to your host computer. Data transfer is done over serial (USB) and Bluetooth. The transmission mode, transmission rate and active channels can all be configured through the API. 

The E3K collects data through its 12-bit ADC, meaning sensor values lie between 0 - 4095. 

To conserve packet size, data is sent in a custom encoded format, where 12-bit integer values occupy 1.5 bytes in a 9 byte array, allowing for a total of 6 transmitted channels. This prevents having to use 16-bit short integers to store the sensor values, which would waste 4 bits per packet. 

## Reference Guide 
### findDevices()
Scans for nearby Bluetooth devices, should be used to check the MAC address of your E3K.

### setState()
Controls whether the datastream is on or off. 

### setCMode()
Controls the transmission mode (serial or Bluetooth).

### setDataRate()
Controls the rate of transmission.

### setChannels()
Controls the active channels in transmission. 
