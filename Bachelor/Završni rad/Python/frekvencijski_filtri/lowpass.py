import cv2
import numpy as np
from matplotlib import pyplot as plt
from fourier import fourierovaTransSlike

"""idealLowPass(img, koef)
- slika se filtrira low-pass filterom
INPUT: slika koja se filtrira, koeficijent filtriranja
OUTPUT: filtrirana slika
"""
def idealLowPass(img, koef):
    #racuna se fourierova transformacija slike
    slika=fourierovaTransSlike(img)
    rows, cols = img.shape
    crow,ccol = int(rows/2) , int(cols/2)
    #stvara se maska, sve vrijednosti su nula osim onih udaljenih
    #od sredista za < koef
    mask = np.zeros((rows,cols,2),np.uint8)
    for i in range(0, rows):
        for j in range(0,cols):
            if (np.sqrt((i-crow)**2 + (j-ccol)**2) < koef):
                mask[i, j] = 1
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

#ovdje se mijenja jacina filtera (drugi argument)
jacina = 10
lowpass = idealLowPass(img, jacina)

#prikazi sliku
low=plt.figure(1)
plt.subplot(121),plt.imshow(img, cmap = 'gray')
plt.title('Ulazna slika'), plt.xticks([]), plt.yticks([])
plt.subplot(122),plt.imshow(lowpass, cmap = 'gray')
plt.title('Filtrirana slika'), plt.xticks([]), plt.yticks([])
low.show()
