#pragma once

#include "hittable.h"
#include "ray.h"

class sphere : public hittable{
    public:
        point3 center;
        double radius;
        
        sphere() {}
        sphere(point3 cen, double r) : center(cen), radius(r) {};

        virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override {
            vec3 oc = r.origin() - center;
            double a = r.direction().length_squared();
            double half_b = dot(oc, r.direction());
            double c = oc.length_squared() - radius*radius;

            double discriminant = half_b*half_b - a*c;
            if (discriminant < 0) return false;
            double sqrtd = sqrt(discriminant);

            // Find nearest root in acceptable range
            double root = (-half_b - sqrtd) / a;
            if (root < t_min || root > t_max){
                root = (-half_b + sqrtd) / a;
                if (root < t_min || root > t_max)
                    return false;
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);

            return true;
        }
};
