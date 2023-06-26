// CHEETAH.cpp : définit le point d'entrée de l'application.
#include "CHEETAH.h"
#include <cstring> 
using namespace std;


int main(int argc, char *argv[])
{	
	int rank, NB_PROCESSES;	
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &NB_PROCESSES);
	
	Trajectory trajectory;
	trajectory.modify(rank);
	
	Serializer srlz = Serializer();
	int local_size = srlz.calculateSerializedSize(trajectory);

	char* buffer = new char[local_size];
	srlz.serialize(trajectory, buffer);
	
	Trajectory trajectory_end;
	srlz.deserialize(buffer, trajectory_end);
	
	
	// Gather the sizes of local trajectories on all processes
    int local_sizes[NB_PROCESSES];
    MPI_Gather(&local_size, 1, MPI_INT, local_sizes, 1, MPI_INT, 0, MPI_COMM_WORLD);


    // Calculate the displacements for Gatherv
    int displacements[NB_PROCESSES];
    int total_size = 0;
    if (rank == 0) {
        for (int i = 0; i < NB_PROCESSES; i++) {
            displacements[i] = total_size;
            total_size += local_sizes[i];
        }
    }

	// Allocate a receive buffer in the main process
    char* receivedBuffer = nullptr;
    if (rank == 0) {
        receivedBuffer = new char[total_size];
    }


	MPI_Gatherv(buffer, local_size, MPI_CHAR, receivedBuffer, local_sizes, displacements, MPI_CHAR, 0, MPI_COMM_WORLD);

	if (rank == 0){		
		// Deserialize the received data
		Trajectory* deserializedObjs = new Trajectory[NB_PROCESSES];
		char* bufferPtr = receivedBuffer;
		for (int i = 0; i < NB_PROCESSES; ++i) {
			srlz.deserialize(bufferPtr, deserializedObjs[i]);
			bufferPtr += local_sizes[i];
		}
		// Print the deserialized objects
        std::cout << "Deserialized objects:" << std::endl;
        for (int j = 0; j < NB_PROCESSES; j++){
			cout << deserializedObjs[j] << endl;
		}

	}
	

	MPI_Finalize();
	
	return 0;
}
