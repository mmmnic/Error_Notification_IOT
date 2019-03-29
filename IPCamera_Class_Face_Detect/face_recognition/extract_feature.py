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


def save_feature(save_path, feature):    
    os.makedirs(os.path.dirname(save_path), exist_ok=True)

    print("[+]Save extracted feature to file : ", save_path)
    np.save(save_path, feature)

def extract_features(src):
    with open(src, "r") as file:
        for i,line in enumerate(file):
            img_path = line[:-1]
            print("[+] Read image  : ", img_path," id : ", i)
            if os.path.isfile(img_path) and img_path.find(".jpg") != -1:            
                save_path = img_path.replace("images", "features").replace(".jpg", ".npy")            
                      
                img_data = cv2.imread(img_path)
                # feature = []
                print("[+] Extract feature from image : ", img_path)
                dets = detector(img_data, 1)

                # Extract feature for one face in one image, if you want to train multi-face
                # in one image, please modify this code by renaming save_path.
                for k, d in enumerate(dets):
                    shape = sp(img_data, d)
                    face_chip = dlib.get_face_chip(img_data, shape)
                    face_descriptor = facerec.compute_face_descriptor(face_chip)
                    # feature.append(face_descriptor)
                    save_feature(save_path, face_descriptor)
                
            

if __name__=="__main__":
    src = sys.argv[1]
    print(src)
    extract_features(src)
