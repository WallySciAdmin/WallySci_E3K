"""
E3K Bluetooth Library
Imports Bluetooth API and includes functions to configure E3K and get samples over serial
Author: Akshat Sahay
"""

import bluetooth  # including pyBluez library

def findDevices():
    """
    :param: None
    :return: None

    Looks for nearby Bluetooth devices, lists their names and MAC addresses
    """
    nearbyDevices = bluetooth.discover_devices(lookup_names=True)  # find list of Bluetooth devices

    if not nearbyDevices:
        print("ERROR: List is empty")

    for addr, name in nearbyDevices:
        print("Address: ", addr)
        print("Name: ", name)


class e3k_bluetooth(object):
    def __init__(self, macAddr, port):
        """
        :param macAddr: MAC address of E3K
        :param port: Port for Bluetooth Communication

        Initialises Bluetooth socket on the given MAC address and socket
        """
        self.macAddr = macAddr
        self.port = port

        self.sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)  # start socket, RFCOMM is communication protocol for Windows and Mac
        self.sock.connect((self.macAddr, self.port))  # connect to E3K

    def sendConfigByte(self, configByte):
        """
        :param configByte: 16-bit number giving configuration details to the device
        :return: None

        Configures E3K by sending over details specified by user (check getConfig())
        """
        self.sock.send(configByte)  # send configByte to start E3K over Bluetooth

    def readSample(self, size):
        """
        :param size: Size of data packet to be read
        :return: rawData: Data in bytes format to be unpacked and decoded (check readData())

        Collects a single data sample and returns it in raw format
        """
        return self.sock.recv(size)

