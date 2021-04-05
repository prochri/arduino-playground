#!/usr/bin/env python
import time
import random

import PyCmdMessenger

arduino = PyCmdMessenger.ArduinoBoard('/dev/ttyACM0', baud_rate=115200)

commands = [
    ["kAcknowledge", ""],
    ["kError", "s"],
    ["kSetLED", "ssss"],
    ["kTest", ""],
]

c = PyCmdMessenger.CmdMessenger(arduino, commands)

numpixels = 5
random.seed(69)

while True:
    l = [0]
    for _ in range(3):
        l.append(random.randint(0,255))
    for i in range(numpixels):
        l[0] = i
        c.send("kSetLED", *l)
    time.sleep(0.5)