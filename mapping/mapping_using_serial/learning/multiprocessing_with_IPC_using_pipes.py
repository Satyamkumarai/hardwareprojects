import multiprocessing
import os 
val = 100
from time import sleep
def sender(conn,msgs):
    print(f"Sender started with PID : {os.getpid()}")
    print(f"Sender: My Parent is {os.getppid()}")
    print("Value : ",val)
    for msg in msgs:
        conn.send(msg) 
        print(f"Sender sent the message {msg}")
    conn.close()

def reciever(conn):
    print(f"Reciever started with PID : {os.getpid()}")
    print(f"Reciever: My Parent is {os.getppid()}")

    while 1 :
        msg = conn.recv()
        sleep(2)
        print("Value : ",val)

        if msg == 'END':

            print("END : Stopping Reciever ")
            break
        print(f"Message Recieved {msg}")

if __name__=='__main__':
    print(f"Main Parent  Process PID : {os.getppid()}")
    print(f"Main Process PID : {os.getpid()}")
    msgs = [1,23.2,"Hello",{1:2,3:4}] *10 + ['END']
    sender_conn,reciever_conn = multiprocessing.Pipe()
    p1 = multiprocessing.Process(target=sender,args=(sender_conn,msgs))
    p2 = multiprocessing.Process(target=reciever,args=(reciever_conn,))
    print(f" {'-'*100}\nStarting P1")
    p1.start()
    print(f" {'-'*100}\nStarting P2")
    p2.start()
    print(f" {'-'*100}\nStarting P2")

    p1.join()
    p2.join()




