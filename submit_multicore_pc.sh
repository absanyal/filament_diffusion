MAX_JOBS=128

wait_for_jobs() {
    while (( $(jobs -p | wc -l) >= MAX_JOBS )); do
        sleep 1  # Check every second for a free slot
    done
}

make clean
make

rm -f input_files/*

n_sims=128

python3 input_writer_pc.py ${n_sims}

rm -rf runs
mkdir -p runs

for i in $(seq 1 ${n_sims})
do
    wait_for_jobs
    echo "Submitting simulation ${i} / ${n_sims}"
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
    
    (
    ./filaments input.${i}.inp > out.run.${i}
    echo "Simulation ${i} complete"
    ) &

    cd ../..
done
wait
echo "All runs complete"
