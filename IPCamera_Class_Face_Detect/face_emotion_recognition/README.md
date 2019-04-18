# face_emotion_recognition
----
# Introduction
----
This project using HOG + SVM for face detection and Xception model for face emotion recognition.
# Directory Structure
----
```
face_emotion_recognition
-fer2013
--fer2013.csv
-models
--_mini_XCEPTION.106-0.65.hdf5
-test
--images for testing
-train.py
-test.py
-real_time_face_emotion_recognition.py
```
# Usage
----
## 1. Face emotion dataset:
Click this [link](https://www.kaggle.com/c/challenges-in-representation-learning-facial-expression-recognition-challenge/data) to download face emotion dataset (fer2013), extract it and put into folder fer2013.
## 2. Training:
Using [this script](https://github.com/fxanhkhoa/IPCamera_Class_Face_Detect/blob/master/face_emotion_recognition/train.py) to train with Xception model. The syntax as following:
```
python3 train.py
```
It will create weight of model in models.
## 3. Testing:
Using [this script](https://github.com/fxanhkhoa/IPCamera_Class_Face_Detect/blob/master/face_emotion_recognition/test.py) to predict your owm image with trained model. The syntax as following:
```
python3 test.py <PATH_OF_IMAGE>
```
It will show label of face emotion on image.
## 4. Streaming realtime from camera:
Using [this script](https://github.com/fxanhkhoa/IPCamera_Class_Face_Detect/blob/master/face_emotion_recognition/real_time_face_emotion_recognition.py) to stream from camera and pass into trained model (default cam 0). The syntax as following:
```
python3 real_time_face_recognition.py <address>
```

# References
----

## Documentations

* [Fer2013 dataset](https://www.kaggle.com/c/challenges-in-representation-learning-facial-expression-recognition-challenge/data)
* [Mini_Xception model](https://arxiv.org/pdf/1710.07557.pdf)
* [Demonstration of Facial Emotion Recognition on Real Time Video Using CNN : Python & Keras](https://appliedmachinelearning.blog/2018/11/28/demonstration-of-facial-emotion-recognition-on-real-time-video-using-cnn-python-keras/)
* [Training a TensorFlow model to recognize emotions](https://medium.com/@jsflo.dev/training-a-tensorflow-model-to-recognize-emotions-a20c3bcd6468)
* [Achieving 90% accuracy in Object Recognition Task on CIFAR-10 Dataset with Keras: Convolutional Neural Networks](https://appliedmachinelearning.blog/2018/03/24/achieving-90-accuracy-in-object-recognition-task-on-cifar-10-dataset-with-keras-convolutional-neural-networks/)
* [Emotion recognition using deep convolutional neural networks](https://github.com/isseu/emotion-recognition-neural-networks/blob/master/paper/Report_NN.pdf)
* [Batch normalization](https://viblo.asia/p/vanishing-exploding-gradients-problems-in-deep-neural-networks-part-2-ORNZqPEeK0n)
* [Pretrained models](https://www.google.com/search?client=ubuntu&channel=fs&q=residual+block+là+gì&ie=utf-8&oe=utf-8)
* [Dlib](http://dlib.net/)

