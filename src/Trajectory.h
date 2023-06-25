#pragma once
#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include "Vector3.h"
#include <vector>
using namespace std;

class Trajectory {
public:
	int rank;
    vector<Vector3> trajectory;
    
    Trajectory();
    Trajectory(int myrank, vector<Vector3> v);
    void modify(int myrank);
    friend ostream& operator<<(ostream& out, const Trajectory& t);

};

#endif