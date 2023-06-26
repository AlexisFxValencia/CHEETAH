to compile : 
cmake -S . -B build
cmake --build build

to run with mpi : 
mpirun -np 4 build/src/CHEETAH

to test :
cd build && ctest



if using on slurm, watch out the gcc version (for the google tests to work): 
for me : 
salloc --partition=dev --nodes=1 --ntasks-per-node=4 --time=100 
module load cmake
module load openmpi
module load gcc/9.4.0

+ C++ version at least 14.