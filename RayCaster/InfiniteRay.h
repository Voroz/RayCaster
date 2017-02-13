#pragma once
#include "Ray.h"


class InfiniteRay : public Ray {
public:
	InfiniteRay(sf::Vector2f p, float angle);
	sf::Vector2f directionVector();
	RayType identify();

	float angle;
};
