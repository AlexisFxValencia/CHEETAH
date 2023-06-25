#include "Trajectory.h"
Trajectory::Trajectory () {
	rank = 1;
	Vector3 position;
	position.x = (double)100*rank;
	position.y = (double)100*rank + 1;
	position.z = (double)100*rank + 2;
	trajectory.push_back(position);
	trajectory.push_back(position);
	trajectory.push_back(position);
}

Trajectory::Trajectory (int myrank, vector<Vector3> v){
	rank = myrank;
	for (int i = 0; i < v.size(); i++) {
		trajectory.push_back(v[i]);
	}
}
