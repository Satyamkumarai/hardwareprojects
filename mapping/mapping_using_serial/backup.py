import serial
import numpy as np
import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import cv2
from matplotlib import cm
from matplotlib.colors import ListedColormap, LinearSegmentedColormap
from queue import Queue

#--------------Change These ------------------------
R,C = 3 , 7                              #number of pins
BAUDE_RATE = 115200                #baud Rate 
PORT = 'COM8'                       #Com Port
shape = (R,C)
vmin = 60
vmax = 600
# bottom = cm.get_cmap('Oranges_r', 128)
# top = cm.get_cmap('Blues', 128)

# newcolors = np.vstack((top(np.linspace(0, 1, 128)),
#                        bottom(np.linspace(0, 1, 128))))
# newcmp = ListedColormap(newcolors, name='OrangeBlue')                       
cmap = "rainbow"
saveFile = True
filename = "Output.csv"
#---------------------------------------------------
TIME_STEP = 0.1
# if saveFile:
    # file = open(filename,"a")
try : 
    with serial.Serial(PORT,BAUDE_RATE,timeout=None,bytesize=8,parity='N',stopbits=1,xonxoff=0, rtscts=0) as ser:
        fig, ax_lst = plt.subplots()
        data = np.array(ser.readline().decode().split(),dtype=np.int64).reshape(shape)
        heatmap = ax_lst.pcolor(data,cmap=cmap,vmin= vmin,vmax= vmax)
        fig.canvas.draw()
        fig.show()

        while data.size >0:
            try :
                t_start = time.time()
                data = np.array(ser.readline().decode().split(),dtype=np.int64).reshape(shape)
                heatmap = ax_lst.pcolor(data,cmap = cmap,vmax=vmax,vmin=vmin)
                ax_lst.draw_artist(ax_lst.patch)
                ax_lst.draw_artist(heatmap)
                fig.canvas.blit(ax_lst.bbox)
                fig.canvas.flush_events()
                t_end = time.time()
                print("fps = {0}".format(999 if t_end == t_start else 1/(t_end-t_start)))
                
            except ValueError:
                pass
except UnicodeDecodeError:
    print ("Error Decoding ")
    


