#! /bin/python

import math
import serial
import time


def main():
    line = serial.Serial('/dev/ttyACM0', 9600)
    line.write(int(time.time()).to_bytes(4, byteorder='little'))

if __name__ == '__main__':
    main()
