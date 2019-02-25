import numpy as np
import cv2 as cv
import imageio
import os
from pathlib import Path

def create_gif(out_file):
    start = input("Press Space to start recording")
    print("Press Space to stop recording")
    if(start == " "):
        print("GO....!!!")
        #cap = cv.VideoCapture("test_code/swainas.mp4")
        cap = cv.VideoCapture(0)
        cap.set(cv.CAP_PROP_FPS,10)
        images = []
        while(True):
            ret, frame = cap.read()
            if (not ret) or (cv.waitKey(1) & 0xFF == ord(' ')):
                print("Over...")
                break

            res = cv.resize(frame,(100,90))
            images.append(res)
            cv.imshow('frame',frame)
            #cv.imshow('resized frame',res)

        cap.release()
        cv.destroyAllWindows()
        imageio.mimsave(out_file,images)
        print("Saved...")
        
if(__name == '__main__'):      
    print("This is the Dataset maker")
    opt = input("\tPress 1 to capture videos\n\tPress 2 to exit")
    while(opt != '2'):
        ob_num = input("Enter the object number")
        task_num = input("Enter the task number")
        ob_folder = Path(f"dataset/placing/object{ob_num}")
        if not os.path.exists(ob_folder):
            os.makedirs(ob_folder)
        out_file = ob_folder / f"task{task_num}.gif"
        create_gif(out_file)
        opt = input("\tPress 1 to capture videos\n\tPress 2 to exit")
