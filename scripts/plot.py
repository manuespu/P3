import matplotlib.pyplot as plt
import soundfile as sf
import numpy as np
import pandas as pd

senyal, fm = sf.read('./scripts/voiced.wav')
t = np.arange(0, len(senyal)) / fm

f = np.loadtxt('./voiced.f0')
l = np.arange(0,len(f))

fig, axs = plt.subplots(2, 1, constrained_layout=True)
axs[0].plot(t, senyal)
axs[0].set_title('Audio')
axs[0].set_xlabel('Tiempo (s)')

axs[1].plot(l,f)
axs[1].set_title('Autocorrelation')

plt.show()