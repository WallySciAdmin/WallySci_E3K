# WallySci E3K
The E3K is your framework for an intuitive understanding of Bio-signals. Use the kit to explore and build your own Electromyography (EMG), Electrocardiography (ECG) and Electroencephalography (EEG) machine. E3K is your Bio-Sensing kit for a variety of project needs in Robotics, Biomechanics, Biosensing, Physiology, Quantitative Feature Extraction, Real-time data transfer and visualization, Multisensory Data Acquisition or AI frameworks.

This repository features firmware and software for your E3K and host computer respectively. 

## Installation
### Firmware 
The E3K firmware is written in the Arduino framework using the ESP32 MCU's custom libraries. To install it onto your E3K, connect it to your host computer, add the ESP32 Arduino Core through the boards managar inside the Arduino IDE, select the correct COM port and push the firmware on the device. 

### Software 
The E3K API written in Python allows users to add the E3K into their projects without having to deal directly with the hardware/firmware. To use, you first need to add the pySerial and pyBluez modules into your Python project. Then, simply include e3k_API.py in your Python project and look at the example.py script to understand how to use the API. 

If you want to utilise the Bluetooth capabilities of the E3K in your project, make sure your computer has a Bluetooth module connected (either an in-built one or an external USB one). 

For Bluetooth and serial, you must specify the MAC address and serial port respectively. 

## Overview
The E3K firmware and software work together to collect data from the attached EMG, EEG and ECG sensors and send it at high speeds from the device to your host computer. Data transfer is done over serial (USB) and Bluetooth. The transmission mode, transmission rate and active channels can all be configured through the API. 

The E3K collects data through its 12-bit ADC, meaning sensor values lie between 0 - 4095.

To conserve packet size, data is sent in a custom encoded format, where 12-bit integer values occupy 1.5 bytes in a 9 byte array, allowing for a total of 6 transmitted channels. This prevents having to use 16-bit short integers to store the sensor values, which would waste 4 bits per value, or a whole 3 bytes per packet. 

## Reference Guide 
```
# Example code for testing the E3K Python API

from e3k_API import *

if __name__ == '__main__':
    device = E3K('C4:4F:33:17:0E:9B', 1, 'COM4')  # initialise device object
    nData = 1000

    device.setState(1)  # set state to 1: turn on data stream
    device.setCMode(1)  # set cMode to 1: communication mode is Bluetooth
    device.setDataRate(3)  # set dataRate to 1: data rate is 1000Hz
    device.setChannels([1, 1, 1, 1, 1, 1])  # set channels[]: turn on all channels

    print("Collecting sensor data...")
    device.readData(nData)  # get 1000 values from the E3K
    for i in range(0, nData):
        print(device.data[i])
```
### findDevices()
This function, not featured in the example above, scans for nearby Bluetooth devices. It should ideally be used in a scratch file to find the MAC address of your E3K device before adding any E3K functionality to your project. 

### E3K(macAddr, bluetoothPort, serialPortNum)
E3K API class. The three arguments specify the Bluetooth MAC address, Bluetooth port and serial port respectively. 

To find the Bluetooth MAC address, use findDevices. The Bluetooth port can just be left as a 1, but you may change it if required. The serial port number should be found by checking Device Manager and seeing the port utilised by the E3K (under Ports->COM and LPT). 

### E3K.setState(stateValue)
Controls whether the datastream is on or off, or whether the device is actively sending data or not. Parameters are a 0 (stream OFF) or 1 (stream ON). 

### E3K.setCMode(cModeValue)
Controls the transmission mode (serial or Bluetooth). Parameters are a 0 (serial communication) or 1 (Bluetooth communication).

### E3K.setDataRate(dataRateValue)
Controls the transmission rate, or the frequency at which data is sent from the device. Parameters are a 0 (10 Hz), 1 (100 Hz), 2 (500 Hz) or 3 (1000 Hz). 

Due to latencies with serial and Bluetooth on the host's side, the rate of receiving data is slightly slower than the rate at which it is sent from the device. Serial is also slower than Bluetooth.

### E3K.setChannels(channelValue)
Controls the active channels in transmission. The 6-digit argument array can be used to specify which channels the users wants active (1) or inactive (0). 

Note that to conserve packet size, inactive channels' readings (ie., 0s) are not included in the packet. To clarify, say if the requested channels are [1, 1, 1, 0, 0, 1], the received packet will be stored out as [channel_1, channel_2, channel_3, channel_6, 0, 0]. Thus if a user is using n sensors, it's recommended to use the first n channels of the E3K. 

### E3K.readData(nData)
This is the function that actually reads data from the device. The parameter nData specifies the number of samples to be read. The samples will all be stored in E3K.data. 
