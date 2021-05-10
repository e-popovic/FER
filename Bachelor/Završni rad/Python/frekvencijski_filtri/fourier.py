import cv2
import numpy as np

"""fourierovaTransSlike(img)
https://docs.opencv.org/master/de/dbc/tutorial_py_fourier_transform.html
- racuna se Fourierova transformacija slike
INPUT: slika koja se filtrira
OUTPUT: fourierova transformacija, slika u frekvencijskoj domeni
"""
def fourierovaTransSlike(img):
    #fourierova transformacija u obliku kompleksnog polja, prvi element predstavlja DC komponentu
    dft = cv2.dft(np.float32(img),flags = cv2.DFT_COMPLEX_OUTPUT)
    #DC element je u sredini polja
    dft_shift = np.fft.fftshift(dft)
    #slika u frekvencijskoj domeni
    frek_domena = 20*np.log(cv2.magnitude(dft_shift[:,:,0],dft_shift[:,:,1]))
    rezultat = [dft_shift, frek_domena]
    return rezultat
