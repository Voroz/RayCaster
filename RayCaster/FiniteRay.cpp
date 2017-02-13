#include "FiniteRay.h"


FiniteRay::FiniteRay(sf::Vector2f p1, sf::Vector2f p2) :
	Ray(p1)
	, p2(p2)
{

}

FiniteRay::FiniteRay(sf::Vector2f p1, float angle, float length) :
	Ray(p1)
{
	p2.x = p1.x + cosf(angle)*length;
	p2.y = p1.y + sinf(angle)*length;
}

sf::Vector2f FiniteRay::directionVector() {
	return sf::Vector2f(p2.x - p1.x, p2.y - p1.y);
}

RayType FiniteRay::identify() {
	return RayType::Finite;
}
