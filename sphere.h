#pragma once

#include "hittable.h"
#include "ray.h"

class sphere : public hittable{
    public:
        point3 center;
        double radius;
        
        sphere() {}
        sphere(point3 cen, double r) : center(cen), radius(r) {};

        virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

        bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const{
            vec3 oc = r.origin() - center;
            double a = r.direction().length_squared();
            double half_b = dot(oc, r.direction());
            double c = oc.length_squared() - radius*radius;

            double discriminant = half_b*half_b - a*c;
            if (discriminant < 0) return false;
            double sqrtd = sqrt(discriminant);

            // Find nearest root in acceptable range
            double root = (-half_b - sqrtd) / a;
            if (root < t_min || t_max < root){
                root = (-half_b + sqrtd) / a;
                if (root < t_min || t_max < root)
                    return false;
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            rec.normal = (rec.p - center) / radius;

            return true;
        }
};
