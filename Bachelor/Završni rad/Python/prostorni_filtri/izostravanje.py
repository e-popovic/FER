import cv2
import numpy as np
from matplotlib import pyplot as plt

#ucitavanje slike
img_big = cv2.imread('../slike/fontana.jpg')
img = cv2.resize(img_big,None,fx=0.3, fy=0.3, interpolation =cv2.INTER_CUBIC)
img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

#stvaranje maski
#1. izostravanje
kernel_sharpen_1 = np.array([[-1,-1,-1], [-1,9,-1], [-1,-1,-1]])
#2. pretjerano (excessive) izostravanje
kernel_sharpen_2 = np.array([[1,1,1], [1,-7,1], [1,1,1]])
#3. poboljsanje rubova - Gaussova maska
kernel_sharpen_3 = np.array([[-1,-1,-1,-1,-1],
[-1,2,2,2,-1],
[-1,2,8,2,-1],
[-1,2,2,2,-1],
[-1,-1,-1,-1,-1]]) / 8.0

#filtriranje slika
out1 = cv2.filter2D(img, -1, kernel_sharpen_1)
out2 = cv2.filter2D(img, -1, kernel_sharpen_2)
out3 = cv2.filter2D(img, -1, kernel_sharpen_3)

#prikaz slika
plt.subplot(141),plt.imshow(img)
plt.title('Ulazna slika'), plt.xticks([]), plt.yticks([])
plt.subplot(142),plt.imshow(out1)
plt.title('Izoštravanje'), plt.xticks([]), plt.yticks([])
plt.subplot(143),plt.imshow(out2)
plt.title('Pretjerano izoštravanje'), plt.xticks([]), plt.yticks([])
plt.subplot(144),plt.imshow(out3)
plt.title('Gaussova maska'), plt.xticks([]), plt.yticks([])
plt.show()
