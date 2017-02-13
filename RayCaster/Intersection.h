#pragma once
#include "Ray.h"


class Intersection {
public:
	Intersection(Ray& r1, Ray& r2, sf::Vector2f p);

	Ray& r1;
	Ray& r2;
	sf::Vector2f p;
};
