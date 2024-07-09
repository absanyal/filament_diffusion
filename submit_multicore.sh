#BSUB -P BIP240
#BSUB -J filament
#BSUB -W 2:00 
#BSUB -nnodes 1
#BSUB -alloc_flags "maximizegpfs smt1 gpumps gpudefault"
### End BSUB Options and begin shell commands

module load gcc

mkdir -p outrun
mkdir -p data
mkdir -p dump

make clean
make

nres=1050

for sim_i in {1..${nres}}
do
    jsrun -n 1 -a1 -c1 -g0 -r1  ./filament input_files/input.${sim_i}.in > outrun/out.run.${sim_i} &
done
