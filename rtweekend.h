#pragma once 

#include<limits>
#include<cstdlib>

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double(){
    return rand() / (RAND_MAX + 1.0); // [0,1)
}

inline double random_double(double min, double max){
    return min + (max-min)*random_double();
}

inline double clamp(double x, double min, double max){
    if (x < min) return min;
    else if (x > max) return max;
    else return x;
}
