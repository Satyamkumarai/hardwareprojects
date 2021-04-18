import multiprocessing
import os
import numpy as np
import serial
import numpy as np
import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import cv2
from matplotlib import cm
from matplotlib.colors import ListedColormap, LinearSegmentedColormap
from queue import Queue

#---------------------------------------------
#----------------------------------------------


#The Sender only sends the raw data through the pipe


# def read_serial(serial):
#     """Takes in  a serial Object and returns the data read from it """
#     data = np.array(serial.readline().decode().split(),dtype=np.int64).reshape(shape)
#     return data


def save_to_file_process(out_pipe,filename,record=True,file_mode='w'):
    """Gets the data from the out_pipe  and saves it to the file filename"""
    name = f"CONSUMER [{os.getpid()}] :"
    print(f"{name} Process Begin..")
    print(f"{name} My Parent is {os.getppid()}")
    if  record:
        file = open(filename,file_mode)
        lines_written = 0
        try:
            while out_pipe.poll(20):
                try:
                    data_in = out_pipe.recv()
                    if len(data_in) >0:
                        file.write(','.join(map(str,data_in)))
                        file.write('\n')
                        lines_written+=1
                    else:
                        break
                except KeyboardInterrupt:
                    print(f"{name} : NOPE -->> CTRL-C")
            print(f"{name}: <<<DONE Saving>>>")
        except Exception as e:
            print(e)
        except KeyboardInterrupt:
            print(f"{name} : KEYBOARD INTERRUPT Continue..")
        finally:
            file.close()
    else:
        print(f"{name} Recording is DISABLED...")
        print(f"{name} >>> Exited successfully...")
    