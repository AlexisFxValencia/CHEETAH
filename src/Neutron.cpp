#include "Neutron.h"
# define M_PI           3.14159265358979323846

Neutron::Neutron() {
    this->position = Vector3(0,0,0);
    this->direction = Vector3(M_PI / 4, M_PI / 4, 0.0);
    this->energy = 2;
    this->max_boundaries = Vector3(34,19, 30);
    this->min_boundaries = Vector3(0,0,0);
    this->previous_position = position;

    is_source = false;
    free_path = 1.0;
    containter_index = -1;
}


Neutron::Neutron(Vector3 position, Vector3 direction, double energy, Vector3 max_boundaries, Vector3 min_boundaries){
    //ctor
    this->position = position;
    this->direction = direction;
    this->energy = energy;
    this->max_boundaries = max_boundaries;
    this->min_boundaries = min_boundaries;
    this->previous_position = position;
    this->is_source = false;
    this->free_path = 1.0;
    this->containter_index = -1;
}


void Neutron::set_random_position() {
    double alea_1 = ((double)rand()/(RAND_MAX + 1));
    this->position.x = alea_1 * (max_boundaries.x - min_boundaries.x) + min_boundaries.x;
    this->previous_position.x = position.x;

    double alea_2 = ((double)rand() / (RAND_MAX + 1));
    this->position.y = alea_2 * (max_boundaries.y - min_boundaries.y) + min_boundaries.y;
    this->previous_position.y = position.y;

    double alea_3 = ((double)rand() / (RAND_MAX + 1));
    this->position.z = alea_3 * (max_boundaries.z - min_boundaries.z) + min_boundaries.z;
    this->previous_position.z = position.z;
}


void Neutron::take_one_step() {    
    Vector3 step = this->direction * free_path;
    this->position = position + step;
}

void Neutron::rebound_on_walls() {
    if (position.x > max_boundaries.x) {
        direction.x = -abs(direction.x);
    }
    else if (position.x < min_boundaries.x) {
        direction.x = abs(direction.x);
    }
    if (position.y > max_boundaries.y) {
        direction.y = -abs(direction.y);
    }
    else if (position.y < min_boundaries.y) {
        direction.y = abs(direction.y);
    }
    if (position.z > max_boundaries.z) {
        direction.z = -abs(direction.z);
    }
    else if (position.z < min_boundaries.z) {
        direction.z = abs(direction.z);
    }
}


