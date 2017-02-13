#pragma once
#include "Ray.h"


class FiniteRay : public Ray {
public:
	FiniteRay(sf::Vector2f p1, sf::Vector2f p2);
	FiniteRay(sf::Vector2f p1, float angle, float length);
	sf::Vector2f directionVector();
	RayType identify();

	sf::Vector2f p2;
};

