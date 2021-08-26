#ifndef PLANEH
#define PLANEH

#include "hitable.h"

class plane : public hitable {
public:
	plane() {}
	plane(vec3 abc, float m) : abc(abc), m(m) {} // ax + by + cz + m = 0
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	vec3 abc;
	float m;
};

#endif
