from matplotlib.ticker import ScalarFormatter
import numpy as np
import sys
import modules.rcparams


###################################################################


n_sims_total = 1
use_random_seed = True


seed=12344

length = 20
radius = 2.5
mass = 1.0

T = 310.0

wall_collisions = 1

manual_diffusion_constants = 0

D_perp = 0.0
D_par = 0.0
D_rot = 0.0

eta = 0.6913

xlo = -500.0
xhi = 500.0
cell_radius = 350.0

steps_to_skip = 100

kdt = 0.01
dt = 1.0
kreact = kdt / dt
P = kreact * dt

iterations = 10_000_000



###################################################################

# Input file writer

if use_random_seed:
    seed_str = str(np.random.randint(0, n_sims_total * 100000))
else:
    seed_str = str(seed)

# Write the input file
with open("input_test.inp", "w") as file:

    file.write("simulation_number={}\n".format(1))
    file.write("seed={}\n".format(seed_str))

    file.write("\n")

    file.write("length(subunits)={}\n".format(length))
    file.write("radius(nm)={}\n".format(radius))
    file.write("mass(FtsZ mass)={}\n".format(mass))

    file.write("\n")

    file.write("iterations={}\n".format(iterations))
    file.write("dt={}\n".format(dt))

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

    file.write("k_react={}\n".format(kreact))

    file.write("\n")

    file.write("xlo(nm)={}\n".format(xlo))
    file.write("xhi(nm)={}\n".format(xhi))
    file.write("cell_radius(nm)={}\n".format(cell_radius))

    file.write("\n")

    file.write("steps_to_skip={}\n".format(steps_to_skip))
