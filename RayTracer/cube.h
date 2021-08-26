#ifndef CUBEH
#define CUBEH

#include "hitable.h"

class cube : public hitable {
public:
	cube() {}
	cube(vec3 c, float s) : center(c), side(s) {}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	vec3 center;
	float side;
};

bool cube::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	vec3 oc = r.origin() - center;
	return false;
}

#endif