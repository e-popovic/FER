import cv2
from matplotlib import pyplot as plt

#ucitavanje slike
img_big = cv2.imread('../slike/fontana.jpg', cv2.IMREAD_COLOR)
img = cv2.resize(img_big,None,fx=0.3, fy=0.3, interpolation =cv2.INTER_CUBIC)
img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

# ujednacavanje histograma
#crno - bijelo
gray_img = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
heC = cv2.equalizeHist(gray_img)

#u boji
# promjena kanala u YUV, ujednacavanje histograma Y kanala
yuv_img = cv2.cvtColor(img, cv2.COLOR_RGB2YUV)
yuv_img[:,:,0] = cv2.equalizeHist(yuv_img[:,:,0])
# promjena kanala u RGB
heB = cv2.cvtColor(yuv_img, cv2.COLOR_YUV2RGB)

#prikaz slika
c=plt.figure(1)
plt.subplot(121),plt.imshow(gray_img, cmap = 'gray')
plt.title('Ulazna slika'), plt.xticks([]), plt.yticks([])
plt.subplot(122),plt.imshow(heC, cmap = 'gray')
plt.title('Ujednačavanje histograma'), plt.xticks([]), plt.yticks([])
c.show()

b=plt.figure(2)
plt.subplot(121),plt.imshow(img)
plt.title('Ulazna slika'), plt.xticks([]), plt.yticks([])
plt.subplot(122),plt.imshow(heB)
plt.title('Ujednačavanje histograma'), plt.xticks([]), plt.yticks([])
b.show()
