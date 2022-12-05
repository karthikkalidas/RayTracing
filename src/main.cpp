#include <chrono>
#include <iomanip>
#include "rtweekend.h"
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"

color ray_color(const ray& r, const hittable_list& world, int depth) {
    // If ray bounce limit exceeded, no light gathered
    if (depth <= 0) return color(0.0, 0.0, 0.0);
    
    hit_record rec;

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return color(0, 0, 0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    double blend_factor = 0.5*(unit_direction.y() + 1.0);
    return (1.0 - blend_factor) * color(1.0, 1.0, 1.0) + blend_factor * color(0.5, 0.7, 1.0);
}

hittable_list random_scene(){
    hittable_list world;

    auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.items.emplace_back(std::make_shared<sphere>(point3(0, -1000,0), 1000, ground_material));

    for(int a = -11; a < 11; ++a){
        for(int b = -11; b < 11; ++b){
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9){
                std::shared_ptr<material> sphere_material;

                if(choose_mat < 0.8){
                    //diffuse
                    auto albedo = random_vector() * random_vector();
                    sphere_material = std::make_shared<lambertian>(albedo);
                    world.items.emplace_back(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    //metal
                    auto albedo = random_vector(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<metal>(albedo, fuzz);
                    world.items.emplace_back(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    //glass
                    sphere_material = std::make_shared<dielectric>(1.5);
                    world.items.emplace_back(std::make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<dielectric>(1.5);
    world.items.emplace_back(std::make_shared<sphere>(point3(0,1,0), 1.0, material1));

    auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.items.emplace_back(std::make_shared<sphere>(point3(-4,1,0),1, material2));

    auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.items.emplace_back(std::make_shared<sphere>(point3(4,1,0),1, material3));

    return world;
}

int main(){
    auto start = std::chrono::high_resolution_clock::now();

    // Image
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 1200;
    const int image_height = (image_width / aspect_ratio);
    const int samples_per_pixel = 10; 
    const int max_depth = 50;

    // World
    auto world = random_scene();

    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    double dist_to_focus = 10; 
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

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