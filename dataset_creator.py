import numpy as np
import cv2 as cv
import imageio
import os
from pathlib import Path
import time
import serial
import pickle


def create_human_gif(out_file):
    images = []
    start = input("Press Enter to start recording")
    print("Press Space to stop recording")
    if(start == ""):
        print("GO....!!!")
        cap = cv.VideoCapture(0)
        cap.set(cv.CAP_PROP_FPS,10.0)
        fps = cap.get(cv.CAP_PROP_FPS)
        start = time.time()
        while(True):
            ret, frame = cap.read()
            if (not ret) or (cv.waitKey(1) & 0xFF == ord(' ')):
                print("Over...")
                break

            res = cv.resize(frame,(100,90))
            images.append(res)
            cv.imshow('frame',frame)
            #cv.imshow('resized frame',res)
        end = time.time()
        print(end - start)
        cap.release()
        cv.destroyAllWindows()
        imageio.mimsave(out_file,images,duration=0.1)
        print(f"Saved... Frames : {len(images)}")

        
def create_robot_gif(out_file):
    ser = serial.Serial("/dev/ttyUSB0",115200)
    images = []
    actions = []
    robot_dict = {}
    states = [[60,150,35,140,85,80]]
    
    start = input("Press Enter to start recording")
    print("Press Space to stop recording")
    
    if(start == ""):
        ser.write(b' ')
        print("GO....!!!")
        cap = cv.VideoCapture(0)
        cap.set(cv.CAP_PROP_FPS,10.0)
        fps = cap.get(cv.CAP_PROP_FPS)
        start = time.time()
        while(True):
            ret, frame = cap.read()
            if (not ret) or (ser.readline() == "END"):
                print("Over...")
                ind = ser.readline()
                break
                
            res = cv.resize(frame,(100,90))
            images.append(res)
            cv.imshow('frame',frame)
            #cv.imshow('resized frame',res)
            
        end = time.time()
        print(end - start)
        for i in range(ind):
            act_i = ser.readline()
            act_i = [int(x) for x in act_i.split(' ')]
            actions.append(act_i)
            states.append(act_i)
        cap.release()
        cv.destroyAllWindows()
        ser.close()
        imageio.mimsave(out_file,images,duration=0.1)
        print(f"Saved... Frames : {len(images)}")
        robot_dict['demoA'] = actions
        robot_dict['demoS'] = states
        dict_file = open(out_file.with_suffix(".pkl"),w)
        pickle.dump(robot_dict,dict_file)
        dict_file.close()
        print("Pickle file saved...")
        
if(__name__ == '__main__'):      
    print("This is the Dataset maker")
    opt = input("\tPress 1 to capture videos\n\tPress 2 to exit")
    while(opt != '2'):
        ob_num = input("Enter the object number")
        task_num = input("Enter the task number")
        isRobot = input("Enter 'y' if this is robot")
        ob_folder = Path(f"dataset/placing/object{ob_num}")
        if not os.path.exists(ob_folder):
            os.makedirs(ob_folder)
        
        if(isRobot == 'y'):
            task_folder = ob_folder / "robot"
            create_data = create_robot_gif
        else:
            task_folder = ob_folder / "human"
            create_data = create_human_gif
            
        if not os.path.exists(task_folder):
            os.makedirs(task_folder)
            
        out_file = task_folder / f"task{task_num}.gif"
        create_data(out_file)
        opt = input("\tPress 1 to capture videos\n\tPress 2 to exit")
