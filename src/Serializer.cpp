#include "Serializer.h"

Serializer::Serializer(){
    
}

int Serializer::calculateSerializedSize(const Trajectory& obj){
	int size = 0;

    // Size of rank
    size += sizeof(int);

    // Size of trajectory size
    size += sizeof(size_t);

    // Size of each Vector3 element in trajectory
    size += obj.trajectory.size() * sizeof(double) * 3;

    return size;
}



// Helper function for serialization
void Serializer::serialize(const Trajectory& obj, char* buffer) {
    int* intBuffer = reinterpret_cast<int*>(buffer);
    size_t* sizeBuffer = reinterpret_cast<size_t*>(buffer + sizeof(int));
    double* doubleBuffer = reinterpret_cast<double*>(buffer + sizeof(int) + sizeof(size_t));

    // Serialize rank
    *intBuffer++ = obj.rank;

    // Serialize trajectory size
    *sizeBuffer++ = obj.trajectory.size();

    // Serialize each Vector3 element in trajectory
    for (const auto& vector : obj.trajectory) {
        *doubleBuffer++ = vector.x;
        *doubleBuffer++ = vector.y;
        *doubleBuffer++ = vector.z;
    }
}

// Helper function for deserialization
void Serializer::deserialize(const char* buffer, Trajectory& obj) {
    const int* intBuffer = reinterpret_cast<const int*>(buffer);
    const size_t* sizeBuffer = reinterpret_cast<const size_t*>(buffer + sizeof(int));
    const double* doubleBuffer = reinterpret_cast<const double*>(buffer + sizeof(int) + sizeof(size_t));

    // Deserialize rank
    obj.rank = *intBuffer++;

    // Deserialize trajectory size
    size_t trajectorySize = *sizeBuffer++;

    // Deserialize each Vector3 element in trajectory
    obj.trajectory.clear();
    for (size_t i = 0; i < trajectorySize; ++i) {
        double x = *doubleBuffer++;
        double y = *doubleBuffer++;
        double z = *doubleBuffer++;
        obj.trajectory.emplace_back(x, y, z);
    }
}
