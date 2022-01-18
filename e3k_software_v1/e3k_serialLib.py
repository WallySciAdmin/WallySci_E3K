"""
E3K Serial Library
Imports serial API and includes functions to configure E3K and get samples over serial
Author: Akshat Sahay
"""

import serial  # including pySerial library

class e3k_serial(object):
    def __init__(self, port):
        """
        :param None
        :return: None

        Initialises serial port on COM port with E3K
        """
        self.serialPort = serial.Serial(port, baudrate=115200, timeout=0.0001)  # serial port, connecting to device

    def sendConfigByte(self, configByte):
        """
        :param configByte: 16-bit number giving configuration details to the device
        :return: None

        Configures E3K by sending over details specified by user (check getConfig())
        """
        self.serialPort.write(configByte)

    def getDelay(self, dataRate):
        """
        :param dataRate: Configuration value for the speed of incoming packets
        :return:
        """
        delayTime = 0.001

        if dataRate == 0:
            delayTime = 1.0
        elif dataRate == 1:
            delayTime = 0.1
        elif dataRate == 2:
            delayTime = 0.01
        elif dataRate == 3:
            delayTime = 0.001

        return delayTime

    def readSample(self, size):
        """
        :param size: Size of data packet to be read
        :return: rawData: Data in bytes format to be unpacked and decoded (check readData())

        Collects a single data sample and returns it in raw format
        """
        return self.serialPort.read(size)
