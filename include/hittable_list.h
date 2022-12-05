#pragma once

#include <memory>
#include <vector>
#include "hittable.h"

class hittable_list : public hittable {
    public: 
        std::vector<hittable> items;

        hittable_list() {}

        void clear() { items.clear(); }

        virtual bool hit(const ray&r, double t_min, double t_max, hit_record& rec) const override{
            bool hit_anything {false};
            double closest_so_far = t_max;

            for (const auto& item : items) {
                if (item.hit(r, t_min, closest_so_far, rec)){
                    hit_anything = true;
                    closest_so_far = rec.t;
                }
            }
            return hit_anything;            
        }
};