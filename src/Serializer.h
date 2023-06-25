#pragma once
#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <iostream>
#include "Trajectory.h"
using namespace std;


class Serializer {


public:
	Serializer();
	int calculateSerializedSize(const Trajectory& obj);
	void serialize(const Trajectory& obj, char* buffer);
	void deserialize(const char* buffer, Trajectory& obj);	
};

#endif