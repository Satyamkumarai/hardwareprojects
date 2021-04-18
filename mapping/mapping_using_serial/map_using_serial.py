import serial
import numpy as np
import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import cv2
from matplotlib import cm
from matplotlib.colors import ListedColormap, LinearSegmentedColormap
from queue import Queue
from threading import Thread
import logging

#--------------Change These ------------------------
R,C = 3 , 7                              #number of pins
BAUDE_RATE = 115200                #baud Rate 
PORT = 'COM8'                       #Com Port
shape = (R,C)
vmin = 60
vmax = 600
cmap = "rainbow"
saveFile = True
filename = "Output.csv"
BUFFER = Queue()
EXIT = False
#---------------------------------------------------
#==========================================================
logging.basicConfig(level=logging.DEBUG,format='(%(threadName)-9s) %(message)s',)

class ConsumerThread(Thread):
    def __init__(self, group=None, target=None, name=None,
                 args=(), kwargs=None, verbose=None):
        super(ConsumerThread,self).__init__()
        self.target = target
        self.name = name
        return
    def run(self):
        logging.debug(f"Started Thread :  {self.name}")
        if saveFile:
            file = open(filename , mode='a')
            while not EXIT :
                if not BUFFER.empty():
                    item = BUFFER.get()
                    file.write(item)
                    file.write('\n')
                    logging.debug('Getting ' + str(item) + ' : ' + str(BUFFER.qsize()) + ' items in queue')
                    # time.sleep(random.random())
            logging.debug(f"Exiting Thread: {self.name}")

        return

class ProducerThread(Thread):
    def __init__(self, group=None, target=None, name=None,
                 args=(), kwargs=None, verbose=None):
        super(ProducerThread,self).__init__()
        self.target = target
        self.name = name
    def run(self):
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
                        raw_data = ser.readline().decode().split()
                        BUFFER.put(",".join(raw_data))
                        data = np.array(raw_data,dtype=np.int64).reshape(shape)
                        heatmap = ax_lst.pcolor(data,cmap = cmap,vmax=vmax,vmin=vmin)
                        ax_lst.draw_artist(ax_lst.patch)
                        ax_lst.draw_artist(heatmap)
                        fig.canvas.blit(ax_lst.bbox)
                        fig.canvas.flush_events()
                        t_end = time.time()
                        logging.debug("fps = {0}".format(999 if t_end == t_start else 1/(t_end-t_start)))
                        
                    except ValueError:
                        pass
                    except KeyboardInterrupt:
                        logging.debug("EXITING...")
                        EXIT = True
                        fileWriter.join()
                        break
        except UnicodeDecodeError:
            print ("Error Decoding ")

            return

#==========================================================
fileWriter = ConsumerThread(name = "fileWriter")
fileWriter.start()
serialReader = ProducerThread(name="serialReader")
serialReader.start()
TIME_STEP = 0.1
