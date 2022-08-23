#pragma once

#include <memory>
#include <vector>
#include "hittable.h"

class hittable_list : public hittable {
    public: 
        std::vector<std::shared_ptr<hittable>> items;

        hittable_list() {}
        hittable_list(std::shared_ptr<hittable> item) { add(item); }   

        void clear() { items.clear(); }

        void add(std::shared_ptr<hittable> item) { items.push_back(item); } 

        virtual bool hit(const ray&r, double t_min, double t_max, hit_record& rec) const override{
            hit_record temp_rec;
            bool hit_anything {false};
            double closest_so_far = t_max;

            for (const auto& item : items) {
                if (item->hit(r, t_min, closest_so_far, temp_rec)){
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }
            return hit_anything;            
        }
};