#ifndef POINTLIGHTH
#define POINTLIGHTH

#include "light.h"

class pointlight : public light {
public:
	pointlight() {}
	pointlight(vec3 cen, vec3 col, float i) :
		light(col, i) {
		center = cen;
	}
	virtual vec3 calculate_light(const hit_record& rec) const;
	vec3 center;
};

vec3 pointlight::calculate_light(const hit_record& rec) const {
	vec3 lightDir = center - rec.p;
	float l = lightDir.length();
	float d = dot(unit_vector(rec.normal), unit_vector(lightDir));
	return d * color * intensity * pow(E, -l);
}

#endif
