import matplotlib.pyplot as plt
import numpy as np

file = open("../pav_2341.f0", 'r')

f0 = []
interval = 0.015        
time = 0.15

for line in file.readlines():
    f0.append(float(line))
    time+=interval
file.close()

file = open("../pav_2341.f0ref", 'r')

f0ref = []
time = 0.015
for line in file.readlines():
    f0ref.append(float(line))
    time+=interval
file.close()

axs[0].plot(f0, 'ro', markersize = 2)
axs[1].plot(f0ref, 'bo', markersize = 2)
fig.suptitle('Comparativa Pitch calculado y de referencia')
fig, axs = plt.subplots(2)
plt.show()