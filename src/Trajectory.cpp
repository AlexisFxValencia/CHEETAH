#include "Trajectory.h"
Trajectory::Trajectory () {
	this->rank = 1;
	Vector3 position;
	position.x = (double)100*rank;
	position.y = (double)100*rank + 1;
	position.z = (double)100*rank + 2;
	this->trajectory.push_back(position);
	this->trajectory.push_back(position);
	this->trajectory.push_back(position);
}

Trajectory::Trajectory (int myrank, vector<Vector3> v){
	this->rank = myrank;
	for (int i = 0; i < v.size(); i++) {
		this->trajectory.push_back(v[i]);
	}
}

void Trajectory::modify(int myrank){
	this->rank = myrank;
	for (int i = 0; i < this->trajectory.size(); i++) {
		this->trajectory[i] = this->trajectory[i]*(double)myrank;
	}
	
}

// Display Trajectory
ostream& operator<<(ostream& out, const Trajectory& t)
{
	out << "rank = " << t.rank << endl;
	for (auto& i : t.trajectory){
		out << "3Vector : " << i;
	}
	
	return out;
}