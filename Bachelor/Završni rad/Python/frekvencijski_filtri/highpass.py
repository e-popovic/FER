import cv2
import numpy as np
from matplotlib import pyplot as plt
from fourier import fourierovaTransSlike

"""idealHighPass(img, koef)
INPUT: slika koja se filtrira, koeficijent filtriranja
OUTPUT: filtrirana slika
"""
def idealHighPass(img, koef):
    #racuna se fourierova transformacija slike
    slika=fourierovaTransSlike(img)
    rows, cols = img.shape
    crow, ccol = int(rows/2) , int(cols/2)
    #stvara se maska, sve vrijednosti su jedan osim onih udaljenih
    #od sredista za < koef
    mask = np.ones((rows,cols,2),np.uint8)
    for i in range(0, rows):
        for j in range(0,cols):
            if (np.sqrt((i-crow)**2 + (j-ccol)**2) < koef):
                mask[i, j] = 0
    #primjena maske (filtera)
    fshift = slika[0]*mask
    #vracanje slike u prostornu domenu
    f_ishift = np.fft.ifftshift(fshift)
    img_back = cv2.idft(f_ishift, flags=cv2.DFT_SCALE | cv2.DFT_REAL_OUTPUT)
    return img_back

"""main"""
#ucitavanje slike
img_big = cv2.imread('../slike/fontana.jpg')
img = cv2.resize(img_big,None,fx=0.3, fy=0.3, interpolation =cv2.INTER_CUBIC)
img = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)

#mijenja se jacina filtera (drugi argument)
jacina = 60
highpass = idealHighPass(img, jacina)
highpass = cv2.convertScaleAbs(highpass)

#prikazi sliku
high=plt.figure(1);
plt.subplot(121),plt.imshow(img, cmap = 'gray')
plt.title('Ulazna slika'), plt.xticks([]), plt.yticks([])
plt.subplot(122),plt.imshow(highpass, cmap = 'gray')
plt.title('High-pass filtrirana slika'), plt.xticks([]), plt.yticks([])
high.show()
