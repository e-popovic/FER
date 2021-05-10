import cv2
import numpy as np
from matplotlib import pyplot as plt
from fourier import fourierovaTransSlike

#ucitavanje slike
img_big = cv2.imread('../slike/fontana.jpg')
img = cv2.resize(img_big,None,fx=0.3, fy=0.3, interpolation =cv2.INTER_CUBIC)
img = cv2.cvtColor(np.float32(img), cv2.COLOR_RGB2GRAY)

fourier = fourierovaTransSlike(img);

#prikazi sliku
plt.subplot(121),plt.imshow(img, cmap = 'gray')
plt.title('Ulazna slika'), plt.xticks([]), plt.yticks([])
plt.subplot(122),plt.imshow(fourier[1], cmap = 'gray')
plt.title('Frek. domena'), plt.xticks([]), plt.yticks([])
plt.show()
