// CHEETAH.cpp : définit le point d'entrée de l'application.
#include "CHEETAH.h"
#include "Neutron.h"
using namespace std;

int main(int argc, char *argv[])
{	
	int rank, size;	
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	int NUM_PROCESSES = size;

	Neutron neutron = Neutron();
	neutron.position.x = (double)100*rank;
	neutron.position.y = (double)100*rank + 1;
	neutron.position.z = (double)100*rank + 2;

	vector<Vector3> local_trajectories_vector;
	local_trajectories_vector.push_back(neutron.position);
	local_trajectories_vector.push_back(neutron.position);
	if (rank ==2){
		local_trajectories_vector.push_back(neutron.position);
	}
	int local_size = local_trajectories_vector.size();	
	Vector3 local_trajectories[local_size];
	for (int i = 0; i < local_size; i++) {
		local_trajectories[i] = local_trajectories_vector[i];
	}

	// Gather the sizes of local trajectories on all processes
    int local_sizes[NUM_PROCESSES];
    MPI_Gather(&local_size, 1, MPI_INT, local_sizes, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate the displacements for Gatherv
    int displacements[NUM_PROCESSES];
    int total_size = 0;
    if (rank == 0) {
        for (int i = 0; i < NUM_PROCESSES; i++) {
            displacements[i] = total_size;
            total_size += local_sizes[i];
        }
    }

	Vector3 all_trajectories[total_size];

	
    MPI_Datatype MPI_Vector3;
    MPI_Type_contiguous(sizeof(Vector3), MPI_BYTE, &MPI_Vector3);
    MPI_Type_commit(&MPI_Vector3);
    MPI_Gatherv(local_trajectories, local_size, MPI_Vector3, all_trajectories, local_sizes, displacements, MPI_Vector3, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		cout << "Array of objects:" << std::endl;
		int count = 0;
		for (int i = 0; i < NUM_PROCESSES; ++i) {
			cout << "Process " << i << ":" << std::endl;
			for (int j = count; j < count+local_sizes[i]; j++) {	
				cout << " Position (x, y, z) : " << all_trajectories[j] << endl;
			}			
			count += local_sizes[i];
		}
		
	}
	
	

	MPI_Finalize();
	
	return 0;
}
