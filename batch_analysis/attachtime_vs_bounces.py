import numpy as np
import matplotlib.pyplot as plt
import os
import sys
import inspect

currentdir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parentdir = os.path.dirname(currentdir)
sys.path.insert(0, parentdir)

import modules.rcparams

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

# fig, ax = plt.subplots(1, 3, figsize=(12, 4))

# ax[0].scatter(attach_times, bounces, s=10, c='k', alpha=0.5)
# ax[0].set_xlabel("Attachment time (ms)")
# ax[0].set_ylabel("Number of bounces")

# ax[1].hist(attach_times, bins='auto', color='k', alpha=0.5)
# ax[1].set_xlabel("Attachment time (ms)")
# ax[1].set_ylabel("Frequency")

# avg_attach_time = np.mean(attach_times)

# ax[1].axvline(avg_attach_time, color='r', linestyle='--', label="Average attach time: {:.2f} ms".format(avg_attach_time))
# ax[1].legend()

# ax[2].hist(bounces, bins='auto', color='k', alpha=0.5)
# ax[2].set_xlabel("Number of bounces")
# ax[2].set_ylabel("Frequency")

# avg_bounces = np.mean(bounces)

# ax[2].axvline(avg_bounces, color='r', linestyle='--', label="Average bounces: {:.2f}".format(avg_bounces))
# ax[2].legend()

# plt.tight_layout()

plt.figure(figsize=(5, 5))

plt.scatter(attach_times, bounces, s=10, c='k', alpha=0.5, lw=0)

plt.yscale('log')
plt.xscale('log')

plt.xlabel("Attachment time (ms)")
plt.ylabel("Number of bounces")

plt.savefig("attachtime_vs_bounces.pdf", format='pdf', bbox_inches='tight')

plt.clf()
plt.cla()

plt.figure(figsize=(5, 5))

plt.hist(attach_times, bins='auto', color='b', alpha=0.5)

plt.xlabel("Attachment time (ms)")
plt.ylabel("Frequency")

avg_attach_time = np.mean(attach_times)

plt.axvline(avg_attach_time, color='r', linestyle='--', label="Average attach time: {:.2f} ms".format(avg_attach_time))

plt.legend()

plt.savefig("attachtime_hist.pdf", format='pdf', bbox_inches='tight')

plt.clf()
plt.cla()

plt.figure(figsize=(5, 5))

plt.hist(bounces, bins='auto', color='b', alpha=0.5)

plt.xlabel("Number of bounces")
plt.ylabel("Frequency")

avg_bounces = np.mean(bounces)

plt.axvline(avg_bounces, color='r', linestyle='--', label="Average bounces: {:.2f}".format(avg_bounces))

plt.legend()

plt.savefig("bounces_hist.pdf", format='pdf', bbox_inches='tight')