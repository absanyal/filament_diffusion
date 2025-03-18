import numpy as np
import matplotlib.pyplot as plt
import os
import sys
import inspect
from scipy.optimize import curve_fit

currentdir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parentdir = os.path.dirname(currentdir)
sys.path.insert(0, parentdir)

import modules.rcparams

#-----------------------------------------------------------------------------------------------

k_react = 0.001
dt = 1.0

def rate(k, t):
    return np.exp(-k*t)

folder_count = 0  # type: int
input_path = "../runs"  # type: str
for folders in os.listdir(input_path):  # loop over all files
    if os.path.isdir(os.path.join(input_path, folders)):  # if it's a directory
        folder_count += 1  # increment counter

sim_count = folder_count
print("Number of simulations: {}".format(sim_count))

attach_times = []
bounces = []

successful_attach_times = 0
for sim_i in range(sim_count):
    sim_num = sim_i + 1
    attach_time, n_bounces = np.loadtxt("../runs/run_{}/data/attach_time.{}.dat".format(sim_num, sim_num), unpack=True)
    if attach_time >= 0:
        attach_times.append(attach_time)
        bounces.append(n_bounces)
        successful_attach_times += 1

print("Number of successful attach times: {}".format(successful_attach_times))


plt.figure(figsize=(5, 5))

plt.scatter(attach_times, bounces, s=10, c='k', alpha=0.5, lw=0)

plt.yscale('log')
plt.xscale('log')

plt.xlabel("Attachment time (ms)")
plt.ylabel("Number of bounces")

plt.savefig("attachtime_vs_bounces.{}.pdf".format(k_react), format='pdf', bbox_inches='tight')

plt.title(r'$k_{{\mathrm{{react}}}} = {:.3f}$, $\mathrm{{d}}t = {:.2f}$'.format(k_react, dt))

plt.clf()
plt.cla()

plt.figure(figsize=(5, 5))

counts, bins = np.histogram(attach_times, bins='auto', density=True)
dx = bins[1] - bins[0]

pdf_area = np.trapz(counts, bins[:-1], dx=dx)

counts = counts / pdf_area
pdf_area = np.trapz(counts, bins[:-1], dx=dx)
print("PDF area: {}".format(pdf_area))

CDF = np.zeros_like(counts)
for i in range(len(counts)):
    CDF[i] = np.trapz(counts[:i+1], bins[:i+1], dx=dx)

survival = 1 - CDF

p_opt, p_cov = curve_fit(rate, bins[:-1], survival, p0=[0.01])

k = p_opt[0]
print("k: {}".format(k))
print("1/k: {}".format(1/k))
print("<t> : {}".format(np.mean(attach_times)))

y_fit = rate(bins[:-1], k)

plt.plot(bins[:-1], survival, 'o', label="Survival function")
plt.plot(bins[:-1], y_fit, 'r-', label=r"$k_{{\mathrm{{attach}}}} = {:.2f}$".format(k))
plt.yscale('log')

plt.title(r'$k_{{\mathrm{{react}}}} = {:.3f}$, $\mathrm{{d}}t = {:.2f}$'.format(k_react, dt))

plt.legend()

plt.savefig("kreact.{}.pdf".format(k_react), format='pdf', bbox_inches='tight')

plt.clf()
plt.cla()

plt.figure(figsize=(5, 5))

plt.hist(attach_times, bins='auto', color='b', alpha=0.5, density=True)

plt.xlabel("Attachment time (ms)")
plt.ylabel("Frequency")

avg_attach_time = np.mean(attach_times)

plt.axvline(avg_attach_time, color='r', linestyle='--', label="Average attach time: {:.2f} ms".format(avg_attach_time))

plt.title(r'$k_{{\mathrm{{react}}}} = {:.3f}$, $\mathrm{{d}}t = {:.2f}$'.format(k_react, dt))

plt.legend()

plt.savefig("attachtime_hist.{}.pdf".format(k_react), format='pdf', bbox_inches='tight')

plt.clf()
plt.cla()

plt.figure(figsize=(5, 5))

plt.hist(bounces, bins='auto', color='b', alpha=0.5, density=True)

plt.xlabel("Number of bounces")
plt.ylabel("Frequency")

avg_bounces = np.mean(bounces)

plt.axvline(avg_bounces, color='r', linestyle='--', label="Average bounces: {:.2f}".format(avg_bounces))

plt.title(r'$k_{{\mathrm{{react}}}} = {:.3f}$, $\mathrm{{d}}t = {:.2f}$'.format(k_react, dt))

plt.legend()

plt.savefig("bounces_hist.{}.pdf".format(k_react), format='pdf', bbox_inches='tight')