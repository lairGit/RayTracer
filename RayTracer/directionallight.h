#ifndef DIRECTIONALLIGHTH
#define DIRECTIONALLIGHTH

#include "light.h"

class directionallight : public light {
public:
	directionallight() {}
	directionallight(vec3 dir, vec3 col, float i) :
		light(col, i) {
		direction = dir;
	}
	virtual vec3 calculate_light(const hit_record& rec) const;
	vec3 direction;
};

vec3 directionallight::calculate_light(const hit_record& rec) const {
	float d = dot(- unit_vector(rec.normal), unit_vector(direction));
	return d * color * intensity;
}

#endif

