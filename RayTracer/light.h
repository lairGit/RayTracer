#ifndef LIGHTH
#define LIGHTH

#include "vec3.h"
#include "hitable.h"

#define E 2.718281828459045

class light {
public:
	light() {}
	light(vec3 col, float i) : color(col), intensity(i) {}
	vec3 color;
	float intensity;
	virtual vec3 calculate_light(const hit_record& rec) const = 0;
};

#endif

