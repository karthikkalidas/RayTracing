#pragma once

#include <iostream>

#include "rtweekend.h"
#include "vec3.h"

void write_color(std::ostream &out, const color pixel_color, const int samples_per_pixel){
    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    double scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    out << static_cast<int>(255 * clamp(r, 0.0, 1.0)) << ' '
        << static_cast<int>(255 * clamp(g, 0.0, 1.0)) << ' '
        << static_cast<int>(255 * clamp(b, 0.0, 1.0)) << '\n';
}
