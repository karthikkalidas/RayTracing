#pragma once

#include <iostream>

#include "rtweekend.h"
#include "vec3.h"

void write_color(std::ostream &out, const color pixel_color, const int samples_per_pixel){
    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    // Divide the color by the number of samples and gamma 2.0 correction.
    double scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}
