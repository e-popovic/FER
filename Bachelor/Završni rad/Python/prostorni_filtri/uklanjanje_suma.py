import cv2
from matplotlib import pyplot as plt

#ucitavanje slike bez suma
img_big = cv2.imread('../slike/capitol.bmp')
img_bez = cv2.resize(img_big,None,fx=0.3, fy=0.3, interpolation =cv2.INTER_CUBIC)
img_bez = cv2.cvtColor(img_bez, cv2.COLOR_BGR2RGB)

#ucitavanje slike sa sumom
img_big = cv2.imread('../slike/sum3.bmp')
img = cv2.resize(img_big,None,fx=0.3, fy=0.3, interpolation =cv2.INTER_CUBIC)
img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

#mean blur filter
blur = cv2.blur(img, (3,3))

#median blur filter
median = cv2.medianBlur(img, 3)

#nelokalni algoritam srednje vrijednosti
b5 = cv2.fastNlMeansDenoisingColored(img,None,5,5,7,21)
#veci faktor uklanjanja 5 -> 10
b10 = cv2.fastNlMeansDenoisingColored(img,None,10,10,7,21)

#odnos signala i suma
bezpsnr = cv2.PSNR(img_bez, img)
print("PSNR (bez uklanjanja suma): {:0.4f} dB".format(bezpsnr))
blurpsnr = cv2.PSNR(img_bez, blur)
print("PSNR (blur): {:0.4f} dB".format(blurpsnr))
medianpsnr = cv2.PSNR(img_bez, median)
print("PSNR (median): {:0.4f} dB".format(medianpsnr))
b5psnr = cv2.PSNR(img_bez, b5)
print("PSNR (nelokalni, faktori 5,5): {:0.4f} dB".format(b5psnr))
b10psnr = cv2.PSNR(img_bez, b10)
print("PSNR (nelokalni, faktori 10,10): {:0.4f} dB".format(b10psnr))

#prikaz slika
b=plt.figure(1)
plt.subplot(321),plt.imshow(img_bez)
plt.title('Bez šuma'), plt.xticks([]), plt.yticks([])
plt.subplot(322),plt.imshow(img)
plt.title('Sa šumom, bez filtra'), plt.xticks([]), plt.yticks([])
plt.subplot(323),plt.imshow(blur)
plt.title('Mean filtar'), plt.xticks([]), plt.yticks([])
plt.subplot(324),plt.imshow(median)
plt.title('Median filtar'), plt.xticks([]), plt.yticks([])
plt.subplot(325),plt.imshow(b5)
plt.title('Nelokalni, faktor=5'), plt.xticks([]), plt.yticks([])
plt.subplot(326),plt.imshow(b10)
plt.title('Nelokalni, faktor=10'), plt.xticks([]), plt.yticks([])
b.show()
