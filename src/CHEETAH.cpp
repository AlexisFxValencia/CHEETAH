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
	/*
	ostream& operator<<(ostream& out, const SizeTrajectories& st) {
		out << st.nb_trajectories << endl;
		for (int i = 0; i < st.nb_trajectories; i++){
			out << " n°" << i << "  : " << st.trajectory_sizes[i] << endl;
		}		
		return out;
	}
	*/

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

	int NB_TRAJECTORIES = 2;
	
	Trajectory trajectory1;
	trajectory1.modify(rank);
	Trajectory trajectory2;
	trajectory2.modify(rank);
	
	Serializer srlz = Serializer();
	int local_size1 = srlz.calculateSerializedSize(trajectory1);
	int local_size2 = srlz.calculateSerializedSize(trajectory2);

	char* buffer1 = new char[local_size1];
	srlz.serialize(trajectory1, buffer1);
	char* buffer2 = new char[local_size2];
	srlz.serialize(trajectory2, buffer2);
	
	//Trajectory trajectory_end;
	//srlz.deserialize(buffer, trajectory_end);
	
	
	// Gather the sizes of local trajectories on all processes
	int local_sizes[NB_TRAJECTORIES+1];
	local_sizes[0] = NB_TRAJECTORIES;
	local_sizes[1] = local_size1;
	local_sizes[2] = local_size2;
	int matrix_size = NB_PROCESSES*(NB_TRAJECTORIES+1);
    int batch_sizes[matrix_size];
    MPI_Gather(&local_sizes, 1*(NB_TRAJECTORIES+1), MPI_INT, batch_sizes, 1*(NB_TRAJECTORIES+1), MPI_INT, 0, MPI_COMM_WORLD);



	SizeTrajectories size_trajectories[NB_PROCESSES];

    // Calculate the displacements for Gatherv    
	int merged_sizes[NB_PROCESSES];
	int total_size = 0;
	int displacements[NB_PROCESSES];
    if (rank == 0) {		
		int matrix_index = 0;
		int process_rank = 0;
		while (matrix_index < matrix_size){
			int nb_trajectories = batch_sizes[matrix_index];
			int current_index = matrix_index;
			matrix_index++;
			size_trajectories[process_rank].nb_trajectories = nb_trajectories;
			merged_sizes[process_rank] = 0;			
			for (int j = 0; j < nb_trajectories; j++){
				merged_sizes[process_rank] += batch_sizes[j + current_index + 1];
				total_size += batch_sizes[j + current_index + 1];
				int a = batch_sizes[j + current_index + 1];
				size_trajectories[process_rank].trajectory_sizes.push_back(a);
				matrix_index++;
			}
			process_rank++;
			
		}	
		
		if (rank ==0){
			for (int i = 0; i < NB_PROCESSES; i++){
				//cout << size_trajectories[i] << endl;
				size_trajectories[i].display();
			}
		}
		
		int sum = 0;
		for (int j = 0; j < NB_PROCESSES; j++){
			displacements[j] = sum;
			sum += merged_sizes[j];
		}
    }
	
	//display_array(rank, displacements, 4);
		
	
	

	// Allocate a receive buffer in the main process
    char* receivedBuffer = nullptr;
    if (rank == 0) {
        receivedBuffer = new char[total_size];
    }
	
	//Merging buffers :
	int merged_size = local_size1 + local_size2;
	char* merged_buffer = new char[merged_size];
	memcpy(merged_buffer, buffer1, local_size1);
	memcpy(merged_buffer + local_size1, buffer2, local_size2);

	if (rank ==0){
		display_array(rank, merged_sizes, 4);
		//cout << merged_size << endl;
	}
	MPI_Gatherv(merged_buffer, merged_size, MPI_CHAR, receivedBuffer, merged_sizes, displacements, MPI_CHAR, 0, MPI_COMM_WORLD);
	
	if (rank == 0){	
		char* bufferPtr = receivedBuffer;
	}
	/*
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
	*/


	
	MPI_Finalize();
	
	return 0;
}
