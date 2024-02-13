import numpy as np
import matplotlib.pyplot as plt
from numpy import log

kB0 = 1.38E-23
T0 = 310
eta0 = 1E-3
d0 = 1E-9

kB = 1
T = 1
eta = 0.6913
d = 5.0

kB = kB * kB0
T = T * T0
eta = eta * eta0
d = d*d0

pi = np.pi


def D_Stokes():
    return (kB * T) / (6 * pi * eta * (d/2))


def D_par(p):
    nu_par = -0.207 + (0.980/p) - (0.133/(p*p))
    return (kB * T * (nu_par + log(p))) / (2 * d * p * pi * eta)


def D_perp(p):
    nu_perp = 0.839 + (0.185/p) + (0.233/(p*p))
    return (kB * T * (nu_perp + log(p))) / (4 * d * p * pi * eta)

def D_rot(p):
    deltarot = -0.622 + 0.917/p - 0.050/(p*p)
    return 3 * kB * T * (deltarot + log(p)) / ( d**3 * p**3 * pi * eta )

def D_rot_0():
    return D_Stokes() / (d0 * d0)

p_list = np.linspace(1, 20, 1000)


j=1.0
print("D_perp =", D_perp(j))
print("D_par =", D_par(j))
print("D_0 =", D_Stokes())
print("D_rot =", D_rot(j))
print("D_rot_0 =", D_rot_0())

D_par_list = D_par(p_list) / D_Stokes()
D_perp_list = D_perp(p_list) / D_Stokes()
D_Stokes_list = np.ones((len(p_list,)))
D_rot_list = D_rot(p_list) / D_rot_0()

plt.plot(p_list, D_Stokes_list)
plt.plot(p_list, D_par_list)
plt.plot(p_list, D_perp_list)
plt.plot(p_list, D_rot_list)
plt.show()

##################################################################

# kB = 1.38E-23
# T = 310
# eta = 0.6913E-3

# PI = 3.1415926535

# d_min = 2
# d_max = 50

# L_list = np.linspace(10, 50, 5)
# d_list = np.linspace(d_min, d_max, 1000)

# for L in L_list:
#     Dpar_list = []
#     for d in d_list:
#         p = (L * 1E-9)/(d * 1E-9)
#         nupar = -0.207 + 0.980/p - 0.133/(p*p)
#         Dpar = (kB * T * (nupar + np.log(p)) /
#                 (2 * (L * 1E-9) * eta * PI)) * (1E12)
#         Dpar_list.append(Dpar)

#     L_str = "{:2.2f}".format(L)

#     plt.plot(d_list, Dpar_list, label=L_str)

#     plt.xlabel(r'$d\,(nm)$')
#     plt.ylabel(r'$D_{\parallel}\,(\mu m^{2} s^{-1})$')
#     plt.xlim(d_min-0.5, d_max+0.5)

# Stokes_list = []

# for d in d_list:
#     Stokes_val = ((kB * T)/(6 * PI * eta * ((d*1E-9)/2))) * (1E12)
#     Stokes_list.append(Stokes_val)

# plt.plot(d_list, Stokes_list, 'k:', label="Stokes")


# plt.legend(title=r'$L\,(nm)$', fancybox=True, loc="best")
# plt.show()

##################################################################
