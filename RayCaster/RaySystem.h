#pragma once
#include <vector>
#include "SFML/Graphics.hpp"
#include "Intersection.h"
#include "Ray.h"
#include "FiniteRay.h"
#include "InfiniteRay.h"


class RaySystem {

public:
	RaySystem();
	~RaySystem();
	void spawnRay(Ray* newRay);
	void trimStart(FiniteRay& toTrim, FiniteRay trimWith);
	void trimEnd(FiniteRay& toTrim, FiniteRay trimWith);
	void renderFiniteRays(sf::RenderWindow& window);
	// Very slow function
	void renderInterSectionPoints(sf::RenderWindow& window);
	bool rayIntersection(Ray& r1, Ray& r2, Intersection& intersection);
	std::vector<FiniteRay*> finiteRays();
	std::vector<InfiniteRay*> infiniteRays();


	std::vector<Ray*> rays;
	std::vector<Intersection> intersections;
};

