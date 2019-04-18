import numpy as np
import sys
import os
from PIL import ImageFile
import cv2
import dlib
from sklearn import svm
from sklearn.externals import joblib
from sklearn import metrics
# import openface

# def pause():
#     programPause = input("Press the <ENTER> key to continue...")

predictor_model = "./weight/shape_predictor_68_face_landmarks.dat"
face_recognition_model = './weight/dlib_face_recognition_resnet_model_v1.dat'

detector = dlib.get_frontal_face_detector()
sp = dlib.shape_predictor(predictor_model)
facerec = dlib.face_recognition_model_v1(face_recognition_model)
# face_aligner = openface.AlignDlib(predictor_model)  

def dlib_to_opencv(rect):
	x = rect.left()
	y = rect.top()
	w = rect.right() - x
	h = rect.bottom() - y
 
	# return a tuple of (x, y, w, h)
	return (x, y, w, h)

def face_recognition(clf):
    cap = cv2.VideoCapture(0)

    while(True):
        ret, frame = cap.read()
        if ret == True:

            dets = detector(frame, 1)  #Xác định vị trí khuôn mặt trong bức ảnh
            # win.add_overlay(dets)   #Vẽ khung hình bao quanh khuôn mặt

            # pause()
            # dlib.hit_enter_to_continue()

            for k, d in enumerate(dets):
                (x, y, w, h) = dlib_to_opencv(d)

                # Xác định facial landmark trên khuôn mặt
                shape = sp(frame, d)

                # Vẽ facial landmark lên bức ảnh
                # win.add_overlay(shape)

                # Affine transformations using openface
                # alignedFace = face_aligner.align(534, img, d, landmarkIndices=openface.AlignDlib.OUTER_EYES_AND_NOSE)

                # Affine transformations using dlib
                face_chip = dlib.get_face_chip(frame, shape)

                # win1 = dlib.image_window()
                # win1.clear_overlay()
                # win1.set_image(face_chip)

                # dlib.hit_enter_to_continue()

                # Encoding faces
                face_descriptor = facerec.compute_face_descriptor(face_chip)
                
                feature = np.reshape(face_descriptor, (1, -1))

                result = clf.predict(feature)[0]
                
                cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 1)
                startX = x
                startY = y - 15 if y - 15 > 15 else y + 15
                cv2.putText(frame, result, (startX, startY), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 255), 2)
                
                print("The face is: " + result)
                # print(face_descriptor)
            cv2.imshow("Recognition", frame)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

    cap.release()
    cv2.destroyAllWindows()

if __name__=="__main__":

    address = sys.argv[1]  #input parameter của script
    db = sys.argv[2]

    clf = joblib.load(db + '/model.joblib')

    face_recognition(clf)
    
