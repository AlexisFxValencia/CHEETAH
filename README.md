to compile : 
cmake -S . -B build
cmake --build build
cd build && ctest


if using on slurm, watch out the gcc version (for the google tests to work): 
for me : module load gcc/9.4.0
+ C++ version at least 14.