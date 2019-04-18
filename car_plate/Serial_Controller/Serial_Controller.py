#!/usr/bin/env python
# coding: utf-8

import threading
import serial
import time


connected = 0
port = 'COM4'
baud = 9600

k = b'hello'.decode(encoding="utf-8")
print(k)

try:
        serial_port = serial.Serial(port, baud, timeout=0)
except Exception as e:
        print(e)

class SerialReadingThread (threading.Thread):
        def __init__(self, threadID, name, serialPort):
                threading.Thread.__init__(self)
                self.serialPort = serialPort
                # self.serial_port.open()
                self.inputstr = ''
                self.count = 0
        def run(self):
                while True:
                        try:
                                reading = self.serialPort.read().decode("utf-8")
                                if ((reading != "")):
                                        self.inputstr = self.inputstr + reading
                                        print(ord(reading))
                                        self.count = self.count + 1
                                        if (reading == '}'):
                                                print(self.count)
                                                print(self.inputstr)
                        except Exception as e:    
                                print(e)
                                return

class SerialWritingThread(threading.Thread):
        def __init__(self, threadID, name, serialPort):
                threading.Thread.__init__(self)
                self.serialPort = serialPort
        
        def run(self):
                while True:
                        try:
                                self.serialPort.write('alo')
                        except Exception as e:
                                print(e)
                        
                        time.sleep(2000)




# Create new threads
thread1 = SerialReadingThread(1, "Thread-1", serial_port)
# threadWrite = SerialWritingThread(2, "ThreadWrite", serial_port)
# Start new Threads
thread1.start()
# threadWrite.start()