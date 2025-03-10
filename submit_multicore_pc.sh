make clean
make

rm -f input_files/*

n_sims=128

python3 input_writer_pc.py ${n_sims}

rm -rf runs
mkdir -p runs

for i in $(seq 1 ${n_sims})
do
    echo "Submitting simulation ${i}"
    mkdir -p runs/run_${i}
    cp input_files_pc/input.${i}.inp runs/run_${i}/input.${i}.inp
    cp filaments runs/run_${i}/filaments
    cd runs/run_${i}
    rm -f outrun/*
    rm -f data/*
    rm -f dump/*

    mkdir -p outrun
    mkdir -p data
    mkdir -p dump

    time ./filaments input.${i}.inp > out.run.${i} &
    cd ../..
done
wait
echo "All runs complete"
