#!/usr/bin/env python
# Generate the serial Sim Data.

import numpy as np
import struct

numFrames = 100
valArray = np.zeros(32).astype('H')
outFile = open('serial_sim.dat', 'w')

for i in np.arange(0,numFrames):
    
    # Output frame header
    frameStr = struct.pack('BBB', 0xFF, 0xFF, 0x00)

    for j in np.arange(0,32):
        frameStr += struct.pack('>H', (valArray[j] & 0x3FF) | (j << 10))
        valArray[j] = (valArray[j] + j + 1) % 1024;

    outFile.write(frameStr)

outFile.close()
