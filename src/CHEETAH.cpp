// CHEETAH.cpp : définit le point d'entrée de l'application.
#include "CHEETAH.h"
#include <cstring> 
using namespace std;

struct SizeTrajectories{
	int nb_trajectories;
    vector<int> trajectory_sizes;
	SizeTrajectories(){
		nb_trajectories = 0;		
	};

	void display(){
		cout << "nb_trajectories for this batch = "<< this->nb_trajectories << endl;
		for (int i = 0; i < this->nb_trajectories; i++){
			cout << "trajectory size in bytes of n°" << i << "  : " << this->trajectory_sizes[i] << endl;
		}	
	}
};

void display_array(int rank, int* displacements, int size) {
    if (rank == 0)  {   
        for (auto it = displacements; it != displacements + size; ++it) {
            std::cout << *it << std::endl;
        }
    }
}

int main(int argc, char *argv[])
{	
	int rank, NB_PROCESSES;	
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &NB_PROCESSES);

	int NB_SOURCES = 13;
	int NB_TRAJECTORIES = NB_SOURCES/NB_PROCESSES;
	if (rank == 0){
		NB_TRAJECTORIES+= NB_SOURCES%NB_PROCESSES;
	}

	Trajectory trajectories[NB_TRAJECTORIES];
	for (int i = 0; i < NB_TRAJECTORIES; i++){
		Trajectory trajectory;
		trajectory.modify(rank);
		trajectories[i] = trajectory;
	}


	Serializer srlz = Serializer();
	
	// Gather the sizes of local trajectories on all processes
	int local_sizes[NB_TRAJECTORIES+1];
	local_sizes[0] = NB_TRAJECTORIES;
	for (int i = 0; i < NB_TRAJECTORIES; i++){
		local_sizes[i+1] = srlz.calculateSerializedSize(trajectories[i]);
	}
	
	int recvcounts[NB_PROCESSES];
	if(rank == 0){
		recvcounts[0] = (NB_SOURCES / NB_PROCESSES) + (NB_SOURCES % NB_PROCESSES);
		recvcounts[0] += 1;
		for (int i = 1; i < NB_PROCESSES; i++){
			recvcounts[i] = NB_SOURCES / NB_PROCESSES;
			recvcounts[i] += 1;
		}
	}
	
	int displs[NB_PROCESSES];	
	if (rank == 0){
		int sum = 0;
		for (int j = 0; j < NB_PROCESSES; j++){
			displs[j] = sum;
			sum += recvcounts[j];
		}
	}
	


	
	int matrix_size = NB_SOURCES + NB_PROCESSES;
    int batch_sizes[matrix_size];
    MPI_Gatherv(&local_sizes, NB_TRAJECTORIES+1, MPI_INT, batch_sizes, recvcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

	
	SizeTrajectories size_trajectories[NB_PROCESSES];
	
	/*
	if (rank == 0){
		for (int i = 0; i<10; i++){
			cout << batch_sizes[i] << endl;
		}
	}
	*/
	
	
	
	
	
	if (rank == 0) {		
		int matrix_index = 0;
		int process_rank = 0;
		while (matrix_index < matrix_size){
			int nb_trajectories = batch_sizes[matrix_index];
			int current_index = matrix_index;
			matrix_index++;
			size_trajectories[process_rank].nb_trajectories = nb_trajectories;		
			for (int j = 0; j < nb_trajectories; j++){
				int one_trajectory_size = batch_sizes[j + current_index + 1];
				size_trajectories[process_rank].trajectory_sizes.push_back(one_trajectory_size);
				matrix_index++;
			}
			process_rank++;			
		}
	}
	
	if (rank == 0){
		for (int i = 0; i < NB_PROCESSES; i++){
			size_trajectories[i].display();
		}
	}
	

	
	
	
	int merged_sizes[NB_PROCESSES];
	int total_size = 0;
	if (rank ==0){
		for (int i = 0; i < NB_PROCESSES; i++){
			merged_sizes[i] = 0;
			for (int j = 0; j < size_trajectories[i].nb_trajectories; j++){
				merged_sizes[i] += size_trajectories[i].trajectory_sizes[j];
				total_size += size_trajectories[i].trajectory_sizes[j];
			}
		}
	}

	int displacements[NB_PROCESSES];
	if (rank == 0){
		int sum = 0;
		for (int j = 0; j < NB_PROCESSES; j++){
			displacements[j] = sum;
			sum += merged_sizes[j];
		}
	}
	
	if (rank == 0){
		for (int k = 0; k < NB_PROCESSES; k++){
			cout << "displacements[k]" << displacements[k] << endl;
		}
	}
	
	
	
	/*
	char* buffer1 = new char[local_size1];
	srlz.serialize(trajectory1, buffer1);
	char* buffer2 = new char[local_size2];
	srlz.serialize(trajectory2, buffer2);
	*/

	char* buffers[NB_TRAJECTORIES];
	for (int i = 0; i < NB_TRAJECTORIES; i++){
		buffers[i] = new char[local_sizes[i+1]];
		srlz.serialize(trajectories[i], buffers[i]);	
		/*	
		for (int j = 0; j < local_sizes[i+1]; j++) {
			cout << "rank : " << rank << "buffers[i] : " << buffers[i] << endl;
		}
		*/
	}
	
	
	// Allocate a receive buffer in the main process
    char* receivedBuffer = nullptr;
    if (rank == 0) {
        receivedBuffer = new char[total_size];
    }
	
	//Merging buffers :
	int merged_size = 0;
	for (int i = 0; i < NB_TRAJECTORIES; i++){
		merged_size += local_sizes[i+1];
	}
	char* merged_buffer = new char[merged_size];
	int cumul = 0;
	for (int i = 0; i < NB_TRAJECTORIES; i++){		
		memcpy(merged_buffer + cumul, buffers[i], local_sizes[i+1]);
		cumul += local_sizes[i+1];
	}
	
	MPI_Gatherv(merged_buffer, merged_size, MPI_CHAR, receivedBuffer, merged_sizes, displacements, MPI_CHAR, 0, MPI_COMM_WORLD);
	
	if (rank == 0){	
		char* bufferPtr = receivedBuffer;
		for (int i = 0; i < NB_PROCESSES; ++i) {
			cout << "Process n " << i << " : " << endl;					
			for (int j = 0; j < size_trajectories[i].nb_trajectories; j++){				
				Trajectory trajectory_end;							
				srlz.deserialize(bufferPtr, trajectory_end);						
				bufferPtr += size_trajectories[i].trajectory_sizes[j];								
				cout << trajectory_end << endl;							
			}			
		}		
	}
	
	
	MPI_Finalize();
	
	return 0;
}
