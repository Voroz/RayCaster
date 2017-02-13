#pragma once
#include "SFML/System.hpp"
#include "RayType.h"


class Ray {
public:
	Ray(sf::Vector2f p1);
	virtual sf::Vector2f directionVector() = 0;
	virtual RayType identify() = 0;

	sf::Vector2f p1;
};

