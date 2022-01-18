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
