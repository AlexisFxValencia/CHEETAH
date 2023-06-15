#pragma once
#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <iostream>
using namespace std;


class Vector3 {


public:
	double x;
	double y;
	double z;
	Vector3();
	Vector3(double x, double y, double z);
	Vector3 operator+(Vector3 v); // ADD 2 Vectors
	Vector3 operator-(Vector3 v); // Subtraction
	double operator^(Vector3 v); // Dot Product
	Vector3 operator*(Vector3 v); // Cross Product
	Vector3 operator*(double a); // scalar multiplication
	double magnitude();
	friend ostream& operator<<(ostream& out, const Vector3& v);	// To output the Vector
};

#endif