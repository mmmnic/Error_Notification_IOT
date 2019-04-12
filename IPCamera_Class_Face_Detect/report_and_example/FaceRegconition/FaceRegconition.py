#!/usr/bin/env python
# coding: utf-8

# In[10]:


import numpy as np
import sys
import os
from PIL import ImageFile
import cv2
import dlib
from sklearn import svm
from sklearn.externals import joblib
from sklearn import metrics
import matplotlib.pyplot as plt


# In[11]:


predictor_model = "./weight/shape_predictor_68_face_landmarks.dat"
face_recognition_model = './weight/dlib_face_recognition_resnet_model_v1.dat'


# In[12]:


detector = dlib.get_frontal_face_detector()
sp = dlib.shape_predictor(predictor_model)
facerec = dlib.face_recognition_model_v1(face_recognition_model)


# In[13]:


def dlib_to_opencv(rect):
    x = rect.left()
    y = rect.top()
    w = rect.right() - x
    h = rect.bottom() - y

    # return a tuple of (x, y, w, h)
    return (x, y, w, h)


# In[22]:


def face_recognition(image_path, clf):
    img = cv2.imread(image_path)
    # print(image_path)

    # win = dlib.image_window()
    # win.clear_overlay()
    # win.set_image(img)

    dets = detector(img, 1)  #Xác định vị trí khuôn mặt trong bức ảnh
    # win.add_overlay(dets)   #Vẽ khung hình bao quanh khuôn mặt

    # pause()
    # dlib.hit_enter_to_continue()

    for k, d in enumerate(dets):
        (x, y, w, h) = dlib_to_opencv(d)

        # Xác định facial landmark trên khuôn mặt
        shape = sp(img, d)

        # Vẽ facial landmark lên bức ảnh
        # win.add_overlay(shape)

        # Affine transformations using openface
        # alignedFace = face_aligner.align(534, img, d, landmarkIndices=openface.AlignDlib.OUTER_EYES_AND_NOSE)

        # Affine transformations using dlib
        face_chip = dlib.get_face_chip(img, shape)

        # win1 = dlib.image_window()
        # win1.clear_overlay()
        # win1.set_image(face_chip)

        # dlib.hit_enter_to_continue()

        # Encoding faces
        face_descriptor = facerec.compute_face_descriptor(face_chip)
        
        feature = np.reshape(face_descriptor, (1, -1))

        result = clf.predict(feature)[0]
        
        cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 3)
        startX = x
        startY = y - 15 if y - 15 > 15 else y + 15
        cv2.putText(img, result, (startX, startY), cv2.FONT_HERSHEY_SIMPLEX, 2, (0, 255, 255), 2)
        
        print("The face is: " + result)
        # print(face_descriptor)
    plt.imshow(img)
#     cv2.imshow("Recognition", img)
#     cv2.waitKey(0)
#     cv2.destroyAllWindows()


# In[23]:


image_path = "images/0/50EC9403-4C2B-4061-A94F-4C19BB2A63AB.jpg"  #input parameter của script
db = "result/db1"

clf = joblib.load(db + '/model.joblib')

face_recognition(image_path, clf)


# In[ ]:




