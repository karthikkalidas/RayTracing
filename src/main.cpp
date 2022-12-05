#include <chrono>
#include <iomanip>
#include "rtweekend.h"
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

color ray_color(const ray& r, const hittable_list& world, int depth) {
    // If ray bounce limit exceeded, no light gathered
    if (depth <= 0) return color(0.0, 0.0, 0.0);
    
    hit_record rec;

    if (world.hit(r, 0.001, infinity, rec)) {
        point3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth-1);
    }
    vec3 unit_direction = unit_vector(r.direction());
    double blend_factor = 0.5*(unit_direction.y() + 1.0);
    return (1.0 - blend_factor) * color(1.0, 1.0, 1.0) + blend_factor * color(0.5, 0.7, 1.0);
}

int main(){
    auto start = std::chrono::high_resolution_clock::now();

    // Image
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = (image_width / aspect_ratio);
    const int samples_per_pixel = 100; 
    const int max_depth = 50;

    // World
    hittable_list world;
    world.items.emplace_back(sphere(point3(0, 0, -1), 0.5));
    world.items.emplace_back(sphere(point3(0, -100.5, -1), 100));

    // Camera
    camera cam;

    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    for(int j = image_height - 1; j >= 0; --j){
        std::cerr<<"\rScanlines remaining: " << j << " " << std::flush;
        for(int i = 0; i < image_width; ++i){
            color pixel_color(0.0, 0.0, 0.0);
            for(int s = 0; s < samples_per_pixel; ++s){
                double u = (i + random_double()) / (image_width - 1);
                double v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            } 
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

    std::cerr << std::fixed << std::showpoint << std::setprecision(3);
    std::cerr << "Execution time:" << duration.count() * 1e-9 << " s\n";
}