make clean
make

python3 input_writer_test.py

echo "Submitting test simulation"
rm -f outrun/*
rm -f data/*
rm -f dump/*

mkdir -p outrun
mkdir -p data
mkdir -p dump

time ./filaments input_test.inp > out.run.test
