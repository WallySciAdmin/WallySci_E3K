"""
E3K Python API
Author: Akshat Sahay
Credit: Varan Gupta, Nanditha Sreenivas, Akshat Sahay
"""

import struct
import time

# custom libraries
from e3k_serialLib import *
from e3k_bluetoothLib import *

class E3K:
    def __init__(self, macAddr, bluetoothPort, serialPortNum):
        """
        :param macAddr: MAC Address for Bluetooth communication
        :param bluetoothPort: Port for Bluetooth communication

        Initialises an instance of the class e3k
        """
        self.dataFormat = 'BBBBBBBBB'  # format for struct unpacking, needs to be variable
        self.size = struct.calcsize(self.dataFormat)  # size of incoming data (in bytes)

        self.sample = [0, 0, 0, 0, 0, 0]  # data collected from the E3K (6 ADC values)
        self.data = []  # all data collected by user in readData

        self.rawData = bytes()  # raw data receieved from the device, needs to be unpacked
        self.unpData = tuple()  # unpacked raw data, needs to be decoded and written into e3k_data

        self.configByte = 0  # 16-bit number which contains all configuration information
        self.state = 1       # controls whether data stream is on or off, values: single bit [ON (1) or OFF (0)]
        self.cMode = 1       # controls the communication mode, values: 2 bits [serial (0), Bluetooth (1), Wi-Fi (2), default (3)]
        self.dataRate = 0    # controls the speed of data stream, values: 2 bits [10 (0), 100 (1), 500 (2), 1000(3)]
        self.channels = [1, 1, 1, 1, 1, 1]  # controls which channel information is coming in data stream [each value per channel]

        self.serialPort = None
        self.bluetoothSocket = None

        self.macAddr = macAddr
        self.bluetoothPort = bluetoothPort
        self.serialPortNum = serialPortNum

    def setState(self, stateValue=0):
        """
        :param stateValue: Client input for state value
        :return:
        """
        self.state = stateValue

    def setCMode(self, cModeValue=0):
        """
        :param cModeValue: Client input for communication mode value
        :return:
        """
        self.cMode = cModeValue

    def setDataRate(self, dataRateValue=0):
        """
        :param dataRateValue: Client input for data rate value
        :return:
        """
        self.dataRate = dataRateValue

    def setDataFormat(self):
        """
        :param: None
        :return: None
        Sets the dataFormat for incoming packet based on number of active channels
        """
        # get number of active channels:
        total = 0
        for x in self.channels:
            total += x

        if total == 0:
            # case might result in error
            self.dataFormat = ''
        elif total == 1:
            self.dataFormat = 'BB'
        elif total == 2:
            self.dataFormat = 'BBB'
        elif total == 3:
            self.dataFormat = 'BBBBB'
        elif total == 4:
            self.dataFormat = 'BBBBBB'
        elif total == 5:
            self.dataFormat = 'BBBBBBBB'
        elif total == 6:
            self.dataFormat = 'BBBBBBBBB'

        self.size = struct.calcsize(self.dataFormat)

    def setChannels(self, channelValue):
        """
        :param channelValue: Client input for channels value
        :return:
        """
        self.channels = channelValue
        self.setDataFormat()

    def calcConfigByte(self):
        """
        :param: None
        :return: None

        Calculates configByte based on configuration variables
        """
        self.configByte |= (self.state << 15) | (self.cMode << 13) | (self.dataRate << 11)
        pos = 10
        for x in self.channels:
            self.configByte |= (x << pos)
            pos -= 1

    def dataDecode(self):
        """
        :param: None
        :return: None

        Decodes self.unpData based on the E3K's encoding format (12-bit numbers mapped over array of bytes)
        """

        machineState = 1  # current state of state machine
        currentChannel = 0  # current channel

        for x in range(0, self.size):
            # state 1: no bits of dataBuffer have been written, no channel is ready
            # write first 8 bits into 8 most significant bits of channel, stay on the same channel and write first 4 bits of the next byte (state 2)
            if machineState == 1:
                self.sample[currentChannel] |= (self.unpData[x] & 0b11111111) << 4

                machineState = 2
                continue

            # state 2: no bits of dataBuffer have been written, channel has 8 bits
            # write first 4 bits into 4 least significant bits of channel, move to the next channel and write last 4 bits of the same byte (state 3)
            if machineState == 2:
                self.sample[currentChannel] |= (self.unpData[x] & 0b11110000) >> 4
                currentChannel += 1

                machineState = 3

            # state 3: 4 bits of dataBuffer have been written, channel has no bits
            # write last 4 bits into 4 most significant bits of channel, stay on the same channel and write 8 bits of next byte (state 4)
            if machineState == 3:
                self.sample[currentChannel] |= (self.unpData[x] & 0b00001111) << 8

                machineState = 4
                continue

            # state 4: no bits of dataBuffer have been written, channel has 4 bits
            # write first 8 bits into 8 least significant bits of channel, move on to the next channel and write 8 bits of the next byte (state 1)
            if machineState == 4:
                self.sample[currentChannel] |= (self.unpData[x] & 0b11111111)
                currentChannel += 1

                machineState = 1
                continue

        # append works off of self.sample's reference
        # to avoid this, we copy self.sample's value into a temp list and append that
        sample = self.sample.copy()
        self.data.append(sample)

        # reset the array
        for x in range(0, self.size):
            self.sample[x] = 0

    def readData(self, nData=10):
        """
        :param nData: number of samples to acquire
        :return: none

        Gets the number of specified samples from the E3K, based on configuration
        """
        self.calcConfigByte()

        if self.cMode == 0:
            # configuration and data stream over serial
            self.serialPort = e3k_serial(self.serialPortNum)  # initialise E3K serial port

            self.serialPort.sendConfigByte(str(self.configByte).encode("utf-8"))  # send current configByte
            time.sleep(1)  # delay for device to respond to configuration

            delay = self.serialPort.getDelay(self.dataRate)  # get required delay based on dataRate

            for x in range(0, nData):
                self.rawData = self.serialPort.readSample(self.size)
                try:
                    self.unpData = struct.unpack(self.dataFormat, self.rawData)  # unpack data according to format
                    self.dataDecode()  # decode data into normal integer formats
                except Exception as E:
                    pass

                time.sleep(0.001)

            self.serialPort.sendConfigByte(b'0')  # reset device (turn off data stream)

        elif self.cMode == 1:
            # configuration and data stream over Bluetooth
            self.bluetoothSocket = e3k_bluetooth(self.macAddr, self.bluetoothPort)  # initialise E3K Bluetooth socket

            self.bluetoothSocket.sendConfigByte(str(self.configByte).encode("utf-8"))  # send current configByte
            time.sleep(1)  # delay for device to respond to configuration

            for x in range(0, nData):
                self.rawData = self.bluetoothSocket.readSample(self.size)
                try:
                    self.unpData = struct.unpack(self.dataFormat, self.rawData)  # unpack data according to format
                    self.dataDecode()  # decode data into normal integer formats

                except Exception as E:
                    pass

            self.bluetoothSocket.sendConfigByte(b'0')  # reset device (turn off data stream)
