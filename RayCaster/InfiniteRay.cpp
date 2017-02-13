#include "InfiniteRay.h"



InfiniteRay::InfiniteRay(sf::Vector2f p, float angle) :
	Ray(p1)
	, angle(angle)
{

}

sf::Vector2f InfiniteRay::directionVector() {
	return sf::Vector2f(cosf(angle), sinf(angle));
}

RayType InfiniteRay::identify() {
	return RayType::Infinite;
}
