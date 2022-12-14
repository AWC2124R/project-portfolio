import os
import cv2
import numpy as np
import random
from keras import layers, models
from keras.applications import VGG16
from keras.applications import VGG19

image_w = 640/10
image_h = 360/10

X = []
Y = []

N = 4000
for idex, categorie in enumerate(['fire', 'normal']):
    label = idex
    image_dir = "train_" + categorie + "_12000/"
    i = 0
    for top, dir, f in os.walk(image_dir):
        for filename in f:
            img = cv2.imread(image_dir+filename)
            img = cv2.resize(img, None, fx=image_w/img.shape[1], fy=image_h/img.shape[0])
            X.append(img)
            Y.append(label)
            i += 1
            if i % 200 == 0:
              print(i)
            if i == N:
                break

np.random.seed(42)

train_images = X[:int(N * 0.8)] + X[int(N):int(N + N * 0.8)]
test_images = X[int(N * 0.8):int(N)] + X[int(N + N * 0.8):int(2 * N)]
train_labels = Y[:int(N * 0.8)] + Y[int(N):int(N + N * 0.8)]
test_labels = Y[int(N * 0.8):int(N)] + Y[int(N + N * 0.8):int(2 * N)]

tmp = list(zip(train_images, train_labels))
np.random.shuffle(tmp)
train_images, train_labels = zip(*tmp)

tmp = list(zip(test_images, test_labels))
np.random.shuffle(tmp)
test_images, test_labels = zip(*tmp)

train_images = np.array(train_images, dtype=np.float64)
test_images = np.array(test_images, dtype=np.float64)
train_labels = np.array(train_labels, dtype=np.float64)
test_labels = np.array(test_labels, dtype=np.float64)
    
train_images = train_images / 255.0
test_images = test_images / 255.0

pre_trained_vgg = VGG19(weights='imagenet', include_top=False, input_shape=(360//10, 640//10, 3))
pre_trained_vgg.trainable = False

model = models.Sequential()
model.add(pre_trained_vgg)
model.add(layers.Flatten())
model.add(layers.Dense(64, activation='relu'))
model.add(layers.Dropout(0.4))
model.add(layers.Dense(32, activation='relu'))
model.add(layers.Dense(2, activation='softmax'))

model.compile(optimizer='adam',\
              loss='sparse_categorical_crossentropy',\
              metrics=['accuracy'])

history = model.fit(train_images, train_labels, epochs=5)

test_loss, test_acc = model.evaluate(test_images, test_labels, verbose=1)
print(test_acc)
