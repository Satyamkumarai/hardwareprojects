import serial
import numpy as np
import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import cv2
from matplotlib import cm
from matplotlib.colors import ListedColormap, LinearSegmentedColormap
from queue import Queue
from helpers import consumer
import multiprocessing
import os
from re import split
import sys
import argparse
from helpers.cmd_interface import *


#--------------Change These ------------------------
parsed = argparser.parse_args()


#this acts as the port
in_file=parsed.filename

is_serial = parsed.serial
BAUDE_RATE = parsed.baudrate                #baud Rate 


out_file=parsed.outfile

record = len(out_file) > 0
mode=parsed.mode


R,C = parsed.rows , parsed.cols                              #number of pins

v_max = parsed.v_max
v_min = parsed.v_min

no_draw = parsed.no_draw


shape = (R,C)
# vmin = parsed.vmin
# vmax = 120
cmap = parsed.cmap




#-------------------------------------------------
def l_print(msg,name):
    print(f'{name} : {msg}')
def producer(in_pipe,file_obj,record=True,name = 'P1',is_serial=True,no_draw=False):
    """The main process where the heatmap is displayed"""
    # draw = True
    p_start_time = time.time()
    name = f"{name} [{os.getpid()}]:"

    #if serail mode set the file to a serial object 
    if is_serial:
        file_obj = serial.Serial(port= file_obj,baudrate=BAUDE_RATE,timeout=None,bytesize=8,parity='N',stopbits=1,xonxoff=0, rtscts=0)
        skip = 0
    else:
        file_obj = open(file_obj,'rb')
        skip = 1
    try : 
        data = np.ones(shape)*-1
        if not no_draw:
            fig, ax_lst = plt.subplots()
            heatmap = ax_lst.pcolor(data,cmap=cmap,vmin= v_min,vmax= v_max)
            fig.canvas.draw()
            fig.show()
        count = 1
        if record:
            in_pipe.send([0] + data.astype(np.float64).flatten().tolist() )
        l_start_time = time.time()
        for line in file_obj:
            try :
                l_delay_time=time.time()-l_start_time
                count+=1
                raw_data = np.array(split(',| ',line.decode().strip()),dtype=np.float64)
                data=raw_data[skip:].reshape(shape)
                if record:
                    in_pipe.send([l_delay_time]+data.astype(np.float64).flatten().tolist()) 
                else:
                    value=raw_data[0]
                    # time.sleep(value)
                if not no_draw:
                    heatmap = ax_lst.pcolor(data,cmap = cmap,vmax=v_max,vmin=v_min)
                    ax_lst.draw_artist(ax_lst.patch)
                    ax_lst.draw_artist(heatmap)
                    fig.canvas.blit(ax_lst.bbox)
                    fig.canvas.flush_events()
                l_end_time = time.time()
                print("{2} Delay = {0} ms\tcount={1}".format(999 if l_end_time == l_start_time else 1/(l_end_time-l_start_time),count,name))
                
            except ValueError as e:
                print(f"{name} ValueERROR", e)
            except UnicodeDecodeError:
                print (f"{name} Error Decoding ")
            # t_start = time.time()
            l_start_time = time.time()
    except KeyboardInterrupt:
        print(f"{name}  KEYBOARD INTERRUPT: Exiting..")
    # in_pipe.send("___E")
    file_obj.close()   
    print(f"{name} >>> Exited sucessfully")





#=====================================================================================================
#                                            MAIN
#=====================================================================================================


if __name__ == '__main__':
    main_name = f'MAIN [{os.getpid()}]'
    #create the pipes only if recording is on..
    if record:
        l_print("RECORDING IS <ON>",main_name)
        
        l_print("Created Pipes",main_name)    
        in_pipe,out_pipe = multiprocessing.Pipe()
    else:
        l_print("RECORDING IS <OFF>",main_name)
        in_pipe,out_pipe = None,None
    # l_print("Created Serial Object",main_name)
    #processes
    process1 = multiprocessing.Process(target=producer,args=(in_pipe,in_file),kwargs={'is_serial':is_serial,'no_draw':no_draw,'record':record})
    process2 = multiprocessing.Process(target=consumer.save_to_file_process,args=(out_pipe,out_file),kwargs={'file_mode':mode,'record':record})

    l_print("Initialized the two Process",main_name)
    
    try:
        process1.start()
        process2.start()

        process1.join()
        process2.join()
    except KeyboardInterrupt:
        l_print("KeyboardInterrupt Exit Sucess",main_name)
