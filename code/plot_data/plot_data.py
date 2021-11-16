import serial
import struct 

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

fig = plt.figure()

ser = serial.Serial('COM8', 115200)

N = 12
data = np.random.rand(N, N)

#im = plt.imshow(data, animated=True)
im = plt.imshow(data, animated=True, cmap=plt.get_cmap('plasma'), interpolation='bilinear',
               vmin=1000, vmax=2000)

def updatefig(*args):
    global data 
    data = ser.read(N*N*2)
    #print(data)
    pstr = '%dH' % (N*N, )
    data = struct.unpack(pstr, data)
    data = np.array(data, dtype=np.uint16).reshape(N, N)
    #print(hex(data[0][0]))
    #data = np.random.rand(8, 8)
    im.set_array(data)
    return im,

ani = animation.FuncAnimation(fig, updatefig, interval=500, blit=True)
plt.show()
