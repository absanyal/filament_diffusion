import numpy as np
import matplotlib.pyplot as plt

kB = 1.38E-23
T = 310
eta = 0.6913E-3

PI = 3.1415926535

d_min = 2
d_max = 50

L_list = np.linspace(10, 50, 5)
d_list = np.linspace(d_min, d_max, 1000)

for L in L_list:
    Dpar_list = []
    for d in d_list:
        p = (L * 1E-9)/(d * 1E-9)
        nupar = -0.207 + 0.980/p - 0.133/(p*p)
        Dpar = (kB * T * (nupar + np.log(p)) /
                (2 * (L * 1E-9) * eta * PI)) * (1E12)
        Dpar_list.append(Dpar)

    L_str = "{:2.2f}".format(L)

    plt.plot(d_list, Dpar_list, label=L_str)

    plt.xlabel(r'$d\,(nm)$')
    plt.ylabel(r'$D_{\parallel}\,(\mu m^{2} s^{-1})$')
    plt.xlim(d_min-0.5, d_max+0.5)

Stokes_list = []

for d in d_list:
    Stokes_val = ((kB * T)/(6 * PI * eta * ((d*1E-9)/2))) * (1E12)
    Stokes_list.append(Stokes_val)

plt.plot(d_list, Stokes_list, 'k:', label="Stokes")


plt.legend(title=r'$L\,(nm)$', fancybox=True, loc="best")
plt.show()
