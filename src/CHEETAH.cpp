// CHEETAH.cpp : définit le point d'entrée de l'application.
//

#include "CHEETAH.h"
#include "Neutron.h"

using namespace std;

int main(int argc, char *argv[])
{
	/*
	cout << "Hello CMake." << endl;
	Neutron neutron = Neutron();
	cout << neutron.max_boundaries.x << endl;
	cout << neutron.position << endl;
	
	for (int i = 0; i < 10; i++) {
		neutron.take_one_step();
		cout << neutron.position << endl;

	}
	*/
	
	int rank, size;	
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
	int NUM_POSITIONS = 1;
	int NUM_PROCESSES = size;
	cout << "my rank is : " <<rank << " over  : " << size << endl;

	Neutron neutron = Neutron();
	neutron.position.x = (double)rank*size;
	neutron.position.y = (double)rank*size+1;
	neutron.position.z = (double)rank*size+2;
	Vector3 allNumbers[NUM_PROCESSES * NUM_POSITIONS];

	MPI_Gather(&neutron.position, NUM_POSITIONS * sizeof(neutron.position), MPI_BYTE, allNumbers, NUM_POSITIONS * sizeof(neutron.position), MPI_BYTE, 0, MPI_COMM_WORLD);

	
	if (rank == 0) {
		cout << "Array of objects:" << std::endl;
		for (int i = 0; i < NUM_PROCESSES; ++i) {
			cout << "Process " << i << ":" << std::endl;
			for (int j = 0; j < NUM_POSITIONS; ++j) {	
				cout << " Position (x, y, z) : " << allNumbers[i * NUM_POSITIONS + j] << endl;
			}
		}
		
	}
	

	MPI_Finalize();
	
	return 0;
}
