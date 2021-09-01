#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <chrono>
#include "sphere.h"
#include "hitablelist.h"
#include "float.h"
#include "camera.h"
#include "pointlight.h"
#include "directionallight.h"
#include "material.h"


vec3 color(const ray& r, hitable* world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
	}
}

vec3 colorWithLights(const ray& r, hitable* objects, light** lights, int num_lights)
{
	hit_record rec;
	vec3 col(0,0,0);
	if (objects->hit(r, 0.001, FLT_MAX, rec)) {
		for (int i = 0; i < num_lights; i++) {
			
			col += lights[i]->calculate_light(rec);
		}
		return col;
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

hitable* random_scene()
{
	int n = 500;
	hitable** list = new hitable * [n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = random0to1();
			vec3 center(a + 0.9 * random0to1(), 0.2, b + 0.9 * random0to1());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8) {
					list[i++] = new sphere(center, 0.2, new lambertian(vec3(random0to1() * random0to1(), random0to1() * random0to1(), random0to1() * random0to1())));
				}
				else if (choose_mat < 0.95) {
					list[i++] = new sphere(center, 0.2, new metal(vec3(0.5 * (1 + random0to1()), 0.5 * (1 + random0to1()), 0.5 * (1 + random0to1())), 0.5*random0to1()));
				}
				else {
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new hitable_list(list, i);
}

int main() {
	auto start_time = std::chrono::system_clock::now();
	int nx = 720;
	int ny = 400;
	int ns = 100;
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	srand(time(NULL));

	hitable* list[4];
	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.15));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
	hitable* world = random_scene();
	//hitable* world = new hitable_list(list, 4);

	light* lights[3];
	lights[0] = new directionallight(vec3(1, -1, -1), vec3(0.7, 0.7, 0.7), 1);
	lights[1] = new pointlight(vec3(-1, 0, -0.5), vec3(0, 1, 0), 1.0);
	lights[2] = new pointlight(vec3(1, 0, -0.5), vec3(0, 0, 1), 1.0);

	vec3 lookfrom(12, 1.5, 3);
	vec3 lookat(4, 1, 1);
	//vec3 lookfrom(6, 1, 1);
	//vec3 lookat(0, 0, 0);
	camera cam(lookfrom, lookat, vec3(0,1,0), 25, float(nx)/float(ny), 0.1, (lookfrom - lookat).length());

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);

			#pragma omp parallel
			{
				#pragma omp for
				for (int s = 0; s < ns; s++) {
					float u = float(i + random0to1()) / float(nx);
					float v = float(j + random0to1()) / float(ny);
					ray r = cam.get_ray(u, v);
					col += color(r, world, 0);
					//col += colorWithLights(r, world, lights, 3);
				}
			}

			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col.r());
			int ig = int(255.99 * col.g());
			int ib = int(255.99 * col.b());
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}

	auto stop_time = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_time = stop_time - start_time;
	//std::cout << "Elapsed seconds: " << elapsed_time.count() << "s\n";
}