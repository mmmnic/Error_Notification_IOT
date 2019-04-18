#!/usr/bin/env python
# coding: utf-8

# In[1]:


import cv2
import numpy as np
import matplotlib.pyplot as plt


# # Scaling

# In[2]:


img = cv2.imread('Audi-logo.jpg', 0)
rows,cols = img.shape

M = np.float32([[1,0,100],[0,1,50]])
dst = cv2.warpAffine(img,M,(cols,rows))

fig = plt.figure()
ax1 = fig.add_subplot(1,2,1)
ax1.imshow(img)
ax2 = fig.add_subplot(1,2,2)
ax2.imshow(dst)

cv2.imshow('raw img', img)
cv2.imshow('img',dst)
cv2.waitKey(0)
cv2.destroyAllWindows()


# # Rotation

# In[4]:


M = cv2.getRotationMatrix2D((cols/2,rows/2),90,1)
dst = cv2.warpAffine(img,M,(cols,rows))

fig = plt.figure()
ax1 = fig.add_subplot(1,2,1)
ax1.imshow(img)
ax2 = fig.add_subplot(1,2,2)
ax2.imshow(dst)

cv2.imshow('raw img', img)
cv2.imshow('img',dst)
cv2.waitKey(0)
cv2.destroyAllWindows()


# # Affine transform

# In[5]:


pts1 = np.float32([[50,50],[200,50],[50,200]])
pts2 = np.float32([[10,100],[200,50],[100,250]])

M = cv2.getAffineTransform(pts1,pts2)

dst = cv2.warpAffine(img,M,(cols,rows))

plt.subplot(121),plt.imshow(img),plt.title('Input')
plt.subplot(122),plt.imshow(dst),plt.title('Output')
plt.show()

cv2.imshow('raw img', img)
cv2.imshow('img',dst)
cv2.waitKey(0)
cv2.destroyAllWindows()


# # Perspective Transform

# In[6]:


pts1 = np.float32([[56,65],[368,52],[28,387],[389,390]])
pts2 = np.float32([[0,0],[300,0],[0,300],[300,300]])

M = cv2.getPerspectiveTransform(pts1,pts2)

dst = cv2.warpPerspective(img,M,(300,300))

plt.subplot(121),plt.imshow(img),plt.title('Input')
plt.subplot(122),plt.imshow(dst),plt.title('Output')
plt.show()

cv2.imshow('raw img', img)
cv2.imshow('img',dst)
cv2.waitKey(0)
cv2.destroyAllWindows()


# In[ ]:




