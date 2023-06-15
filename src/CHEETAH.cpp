// CHEETAH.cpp : définit le point d'entrée de l'application.
//

#include "CHEETAH.h"
#include "Neutron.h"

using namespace std;

int main()
{
	cout << "Hello CMake." << endl;
	Neutron neutron = Neutron();
	cout << neutron.max_boundaries.x << endl;
	cout << neutron.position << endl;
	
	for (int i = 0; i < 10; i++) {
		neutron.take_one_step();
		cout << neutron.position << endl;

	}
	


	return 0;
}
