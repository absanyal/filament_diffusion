from matplotlib.ticker import ScalarFormatter
import numpy as np

import matplotlib as mpl

mpl.rcParams['axes.labelsize'] = 20
mpl.rcParams['xtick.labelsize'] = 20
mpl.rcParams['ytick.labelsize'] = 20
mpl.rcParams['legend.fontsize'] = 14


###################################################################

# Job information

n_cores_per_node = 42

n_cores_per_sim = 1
n_sims_total = 1050

n_sims_per_node = int(n_cores_per_node / n_cores_per_sim)
number_of_empty_cores = n_cores_per_node % n_cores_per_sim

n_nodes_required = int(n_sims_total / n_sims_per_node)

print("Number of cores per simulation: {}".format(n_cores_per_sim))
print("Total number of simulations: {}".format(n_sims_total))
print()
print("Number of sims per node: {}".format(n_sims_per_node))
print("Number of empty cores: {}".format(number_of_empty_cores))
print("Number of nodes required: {}".format(n_nodes_required))

print("---------------------------------")

###################################################################

# Simulation information

print("Setting up constant parameters...")
print("---------------------------------")
# constant parameters are not commented

# seed=12344

length = 20
radius = 2.5
mass = 1.0

# iterations = 10000000
# dt=1

T = 310.0

wall_collisions = 1

manual_diffusion_constants = 0

D_perp = 0.0
D_par = 0.0
D_rot = 0.0

eta = 0.6913

# k_react=0.001

xlo = -500.0
xhi = 500.0
cell_radius = 350.0

steps_to_skip = 100

###################################################################

# Set changing parameters here

print("Setting up changing parameters...")
print("---------------------------------")

kdt = 0.01

dt_list = [1.0]
kreact_list = [kdt / dt for dt in dt_list]
iterations_list = [int(10000000 / dt) for dt in dt_list]
P_list = []
for i in range(len(dt_list)):
    P_list.append(kreact_list[i] * dt_list[i])

print("Required kdt: {}".format(kdt))

print("dt = {}".format(dt_list))
print("kreact = {}".format(kreact_list))
print("P = {}".format(P_list))
print("iterations = {}".format(iterations_list))


###################################################################

# Input file writer

for sim_i in range(1, n_sims_total + 1):

    sim_i_str = str(sim_i)
    seed_str = str(np.random.randint(0, n_sims_total * 100000))

    # Write the input file
    with open("input_files/input.{}.inp".format(sim_i_str), "w") as file:

        file.write("simulation_number={}\n".format(sim_i_str))
        file.write("seed={}\n".format(seed_str))

        file.write("\n")

        file.write("length(subunits)={}\n".format(length))
        file.write("radius(nm)={}\n".format(radius))
        file.write("mass(FtsZ mass)={}\n".format(mass))

        file.write("\n")

        file.write("iterations={}\n".format(iterations_list[0]))
        file.write("dt={}\n".format(dt_list[0]))

        file.write("\n")

        file.write("T(kelvin)={}".format(T))

        file.write("\n")

        file.write("wall_collisions={}\n".format(wall_collisions))

        file.write("\n")

        file.write("manual_diffusion_constants={}\n".format(
            manual_diffusion_constants))

        file.write("\n")

        file.write("D_perp(micro m^2/s)={}\n".format(D_perp))
        file.write("D_par(micro m^2/s)={}\n".format(D_par))
        file.write("D_rot(micro rad^2/s)={}\n".format(D_rot))

        file.write("\n")

        file.write("eta(mPa s)={}\n".format(eta))

        file.write("\n")

        file.write("k_react={}\n".format(kreact_list[0]))

        file.write("\n")

        file.write("xlo(nm)={}\n".format(xlo))
        file.write("xhi(nm)={}\n".format(xhi))
        file.write("cell_radius(nm)={}\n".format(cell_radius))

        file.write("\n")

        file.write("steps_to_skip={}\n".format(steps_to_skip))
