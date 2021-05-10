import cv2
import numpy as np
from matplotlib import pyplot as plt

"""gammaCorrect(image, gamma=1.0)
- za svaki piksel: output = input ^ (1 / gamma)
INPUT: slika, gama faktor
OUTPUT: slika nakon gama korekcije
"""
def gammaCorrect(image, gamma=1.0):
    inverseGamma = 1.0 / gamma
    table = np.array([((i / 255.0) ** inverseGamma) * 255 for i in np.arange(0, 256)]).astype("uint8")
    #pikseli trebaju imati vrijednosti [0,1] -> skaliranje pomocu lookup tablice
    output_img = cv2.LUT(image, table)
    return output_img

"""main"""
#ucitavanje slike
img_big = cv2.imread('../slike/fontana.jpg')
img = cv2.resize(img_big,None,fx=0.3, fy=0.3, interpolation =cv2.INTER_CUBIC)
img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
gray_img = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)

#gama korekcija
#crno - bijelo
c050 = gammaCorrect(gray_img,0.5)
c075 = gammaCorrect(gray_img,0.75)
c125 = gammaCorrect(gray_img,1.25)
c150 = gammaCorrect(gray_img,1.5)

#u boji
b050 = gammaCorrect(img,0.5)
b075 = gammaCorrect(img,0.75)
b125 = gammaCorrect(img,1.25)
b150 = gammaCorrect(img,1.5)


#prikaz slika
c=plt.figure(1)
plt.subplot(151),plt.imshow(c050, cmap = 'gray')
plt.title('Gama = 0.5'), plt.xticks([]), plt.yticks([])
plt.subplot(152),plt.imshow(c075, cmap = 'gray')
plt.title('Gama = 0.75'), plt.xticks([]), plt.yticks([])
plt.subplot(153),plt.imshow(gray_img, cmap = 'gray')
plt.title('Ulazna slika'), plt.xticks([]), plt.yticks([])
plt.subplot(154),plt.imshow(c125, cmap = 'gray')
plt.title('Gama = 1.25'), plt.xticks([]), plt.yticks([])
plt.subplot(155),plt.imshow(c150, cmap = 'gray')
plt.title('Gama = 1.5'), plt.xticks([]), plt.yticks([])
c.show()

b=plt.figure(2)
plt.subplot(151),plt.imshow(b050)
plt.title('Gama = 0.5'), plt.xticks([]), plt.yticks([])
plt.subplot(152),plt.imshow(b075)
plt.title('Gama = 0.75'), plt.xticks([]), plt.yticks([])
plt.subplot(153),plt.imshow(img)
plt.title('Ulazna slika'), plt.xticks([]), plt.yticks([])
plt.subplot(154),plt.imshow(b125)
plt.title('Gama = 1.25'), plt.xticks([]), plt.yticks([])
plt.subplot(155),plt.imshow(b150)
plt.title('Gama = 1.5'), plt.xticks([]), plt.yticks([])
b.show()
