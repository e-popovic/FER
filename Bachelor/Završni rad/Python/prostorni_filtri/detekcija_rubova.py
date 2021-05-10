import cv2
from matplotlib import pyplot as plt

#ucitavanje slike
img_big = cv2.imread('../slike/fontana.jpg', cv2.IMREAD_COLOR)
img = cv2.cvtColor(img_big, cv2.COLOR_RGB2GRAY)
img = cv2.resize(img,None,fx=0.3, fy=0.3, interpolation =cv2.INTER_CUBIC)

#filtriranje opencv funkcijama
#1. sobelov detektor
rows, cols = img.shape
sobel_horizontal = cv2.Sobel(img, cv2.CV_64F, 1, 0, ksize=3)
sobel_vertical = cv2.Sobel(img, cv2.CV_64F, 0, 1, ksize=3)
sobel_horizontal = cv2.convertScaleAbs(sobel_horizontal)
sobel_vertical = cv2.convertScaleAbs(sobel_vertical)
sobel = cv2.addWeighted(sobel_horizontal, 0.5, sobel_vertical, 0.5, 0)

#2. laplaceov filtar
laplacian = cv2.Laplacian(img, cv2.CV_64F)
laplacian = cv2.convertScaleAbs(laplacian)

#3. cannyjev detektor
canny = cv2.Canny(img, 50, 240)

#prikaz slika
c=plt.figure(1)
plt.subplot(231),plt.imshow(img, cmap = 'gray')
plt.title('Ulazna slika'), plt.xticks([]), plt.yticks([])
plt.subplot(232),plt.imshow(laplacian, cmap = 'gray')
plt.title('Laplaceov detektor'), plt.xticks([]), plt.yticks([])
plt.subplot(233),plt.imshow(canny, cmap = 'gray')
plt.title('Cannyjev detektor'), plt.xticks([]), plt.yticks([])

plt.subplot(234),plt.imshow(sobel_horizontal, cmap = 'gray')
plt.title('Sobelov detektor (H)'), plt.xticks([]), plt.yticks([])
plt.subplot(235),plt.imshow(sobel_vertical, cmap = 'gray')
plt.title('Sobelov detektor (V)'), plt.xticks([]), plt.yticks([])
plt.subplot(236),plt.imshow(sobel, cmap = 'gray')
plt.title('Sobelov detektor'), plt.xticks([]), plt.yticks([])

c.show()
