import cv2
from matplotlib import pyplot as plt

#ucitavanje slike
ime_slike = '../slike/fontana.jpg'
img = cv2.imread(ime_slike)
img = cv2.resize(img,None,fx=0.3, fy=0.3, interpolation =cv2.INTER_CUBIC)
img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

#filtriranje opencv funkcijama
#mean blur filter
mean = cv2.blur(img, (7,7))
#median blur filter
median = cv2.medianBlur(img, 7)
#gaussian blur filter
gauss = cv2.GaussianBlur(img, (7,7), 0)

#prikaz slika
plt.subplot(141),plt.imshow(img, cmap = 'gray')
plt.title('Ulazna slika'), plt.xticks([]), plt.yticks([])
plt.subplot(142),plt.imshow(mean, cmap = 'gray')
plt.title('Mean filtar'), plt.xticks([]), plt.yticks([])
plt.subplot(143),plt.imshow(median, cmap = 'gray')
plt.title('Median filtar'), plt.xticks([]), plt.yticks([])
plt.subplot(144),plt.imshow(gauss, cmap = 'gray')
plt.title('Gaussov filtar'), plt.xticks([]), plt.yticks([])
plt.show()
