#pragma once
#ifndef NEUTRON_H
#define NEUTRON_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "Vector3.h"




using namespace std;

class Neutron
{
public:
    Vector3 position;
    Vector3 previous_position;
    Vector3 direction;
    double energy;
    double free_path;
    int containter_index;

    Vector3 max_boundaries;
    Vector3 min_boundaries;

    bool is_source;

    Neutron();
    Neutron(Vector3 position, Vector3 direction, double energy, Vector3 max_boundaries, Vector3 min_boundaries);

    void take_one_step();
    void rebound_on_walls();
    void set_random_position();
    void set_random_velocity(double speed_magnitude);


    void compute_velocity();

    
};

#endif // NEUTRON_H
