import numpy as np
import sys
import os
from PIL import ImageFile
import cv2
import dlib

ImageFile.LOAD_TRUNCATED_IMAGES=True

predictor_model = "./weight/shape_predictor_68_face_landmarks.dat"
face_recognition_model = './weight/dlib_face_recognition_resnet_model_v1.dat'

detector = dlib.get_frontal_face_detector()
sp = dlib.shape_predictor(predictor_model)
facerec = dlib.face_recognition_model_v1(face_recognition_model)

def dlib_to_opencv(rect):
	x = rect.left()
	y = rect.top()
	w = rect.right() - x
	h = rect.bottom() - y
 
	# return a tuple of (x, y, w, h)
	return (x, y, w, h)

def save_image(src):
    with open(src, "r") as file:
        for i,line in enumerate(file):
            img_path = line[:-1]
            print("[+] Read image  : ", img_path," id : ", i)
            if os.path.isfile(img_path) and img_path.find(".jpg") != -1:            
                save_path = img_path.replace("images", "save")            
                      
                img_data = cv2.imread(img_path)
                # feature = []
                print("[+] Extract feature from image : ", img_path)
                dets = detector(img_data, 1)

                # Extract feature for one face in one image, if you want to train multi-face
                # in one image, please modify this code by renaming save_path.
                for k, d in enumerate(dets):
                    (x, y, w, h) = dlib_to_opencv(d)
                    cv2.rectangle(img_data, (x, y), (x + w, y + h), (0, 255, 0), 8)
                os.makedirs(os.path.dirname(save_path), exist_ok=True)
                cv2.imwrite(save_path, img_data)
                # print(save_path)
                # cv2.imwrite(save_path, img_data)
            

if __name__=="__main__":
    src = sys.argv[1]
    save_image(src)
