import matplotlib.pyplot as plt

xpoints = [400, 600, 800, 1000, 1200]
ypoints_blas = [0.060320, 0.144326, 0.253849, 0.509964, 0.841186]
ypoints_neopt = [1.036101, 3.330462, 7.997100, 15.300241, 26.693563]
ypoints_opt = [0.464351, 1.431177, 3.292374, 6.427715, 11.170262]

# plt.subplot(2, 2, 1)
plt.plot(xpoints, ypoints_opt, marker='o')
plt.title('Timpi rulare OPT')
plt.xlabel('N')
plt.ylabel('Timp (s)')
plt.grid()

plt.show()