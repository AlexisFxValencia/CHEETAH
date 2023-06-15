#include "Vector3.h"

Vector3::Vector3() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vector3::Vector3(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

double Vector3::magnitude()
{
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}


// Addition of vectors
Vector3 Vector3::operator+(Vector3 v)
{
	double x1, y1, z1;
	x1 = x + v.x;
	y1 = y + v.y;
	z1 = z + v.z;
	return Vector3(x1, y1, z1);
}

// Subtraction of vectors
Vector3 Vector3::operator-(Vector3 v)
{
	double x1, y1, z1;
	x1 = x - v.x;
	y1 = y - v.y;
	z1 = z - v.z;
	return Vector3(x1, y1, z1);
}

// Dot product of vectors
double Vector3::operator^(Vector3 v)
{
	double x1, y1, z1;
	x1 = x * v.x;
	y1 = y * v.y;
	z1 = z * v.z;
	return (x1 + y1 + z1);
}

// Cross product of vectors
Vector3 Vector3::operator*(Vector3 v)
{
	double x1, y1, z1;
	x1 = y * v.z - z * v.y;
	y1 = z * v.x - x * v.z;
	z1 = x * v.y - y * v.x;
	return Vector3(x1, y1, z1);
}

Vector3 Vector3::operator*(double a)
{
	double x1, y1, z1;
	x1 = x * a;
	y1 = y * a;
	z1 = z * a;
	return Vector3(x1, y1, z1);
}


// Display Vector
ostream& operator<<(ostream& out, const Vector3& v)
{
	out << v.x << "i ";
	if (v.y >= 0)
		out << "+ ";
	out << v.y << "j ";
	if (v.z >= 0)
		out << "+ ";
	out << v.z << "k" << endl;
	return out;
}
