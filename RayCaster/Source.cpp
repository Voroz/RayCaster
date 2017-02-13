#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>


enum RayType {
	Infinite,
	Finite,
};

class Ray {

public:
	Ray(sf::Vector2f p1) :
		p1(p1)
	{

	}
	virtual sf::Vector2f directionVector() = 0;
	virtual RayType identify() = 0;

	sf::Vector2f p1;
};

class FiniteRay : public Ray {

public:
	FiniteRay(sf::Vector2f p1, sf::Vector2f p2) :
		Ray(p1)
		, p2(p2)
	{

	}

	FiniteRay(sf::Vector2f p1, float angle, float length) :
		Ray(p1)
	{
		p2.x = p1.x + cosf(angle)*length;
		p2.y = p1.y + sinf(angle)*length;
	}

	sf::Vector2f directionVector() {
		return sf::Vector2f(p2.x - p1.x, p2.y - p1.y);
	}

	RayType identify() {
		return RayType::Finite;
	}

	sf::Vector2f p2;
};

class InfiniteRay : public Ray {

public:
	InfiniteRay(sf::Vector2f p, float angle) :
		Ray(p1)
		, angle(angle)
	{

	}

	sf::Vector2f directionVector() {
		return sf::Vector2f(cosf(angle), sinf(angle));
	}

	RayType identify() {
		return RayType::Infinite;
	}

	float angle;
};


class Intersection {
public:
	Intersection(Ray& r1, Ray& r2, sf::Vector2f p) :
		r1(r1),
		r2(r2),
		p(p)
	{

	}

	Ray& r1;
	Ray& r2;
	sf::Vector2f p;
};

class RaySystem {

public:
	RaySystem() {

	}

	~RaySystem() {
		for (auto ray : rays) {
			delete ray;
		}
	}

	void spawnRay(Ray* newRay) {
		for (auto ray : rays) {
			Intersection intersection = Intersection(FiniteRay(sf::Vector2f(0, 0), sf::Vector2f(0, 0)), FiniteRay(sf::Vector2f(0, 0), sf::Vector2f(0, 0)), sf::Vector2f(0, 0));
			bool isIntersecting = lineIntersection(*newRay, *ray, intersection);
			if (isIntersecting) {
				intersections.push_back(intersection);
			}
		}
		rays.push_back(newRay);
	}

	void renderFiniteLines(sf::RenderWindow& window) {
		std::vector<FiniteRay> fRays = finiteRays();
		sf::VertexArray vertexArray(sf::PrimitiveType::Lines, fRays.size() * 2);

		for (int i = 0; i < fRays.size(); i++) {
			vertexArray[i * 2] = fRays[i].p1;
			vertexArray[i * 2 + 1] = fRays[i].p2;
		}
		window.draw(vertexArray);
	}

	// Very slow function
	void renderInterSectionPoints(sf::RenderWindow& window) {
		for (auto intersection : intersections) {
			sf::CircleShape circle(3);
			circle.setFillColor(sf::Color::Green);
			circle.setOrigin(circle.getRadius(), circle.getRadius());
			circle.setPosition(intersection.p);
			window.draw(circle);
		}
	}

	bool lineIntersection(Ray& r1, Ray& r2, Intersection& intersection) {
		sf::Vector2f dir1 = r1.directionVector();
		sf::Vector2f dir2 = r2.directionVector();

		float t = 0;
		float mulFactor = 0;

		intersection.r1 = r1;
		intersection.r2 = r2;


		// One of the direction vectors are (0, 0)
		if ((dir1.x == 0 && dir1.y == 0) || (dir2.x == 0 && dir2.y == 0)) {
			return false;
		}

		// Find intersection point of 2 infinite lines
		if (dir2.x != 0 && dir2.y != 0) {
			// r1.p1.x + t*dir1.x = r2.p1.x + s*dir2.x;
			// r1.p1.y + t*dir1.y = r2.p1.y + s*dir2.y;
			mulFactor = -(dir2.y / dir2.x);
			t = (r2.p1.y + r2.p1.x*mulFactor - r1.p1.y - r1.p1.x*mulFactor) / (dir1.y + dir1.x*mulFactor);
			intersection.p = sf::Vector2f(r1.p1.x + t*dir1.x, r1.p1.y + t*dir1.y);
		}
		else if (dir1.x != 0 && dir1.y != 0) {
			// r1.p1.x + s*dir1.x = r2.p1.x + t*dir2.x;
			// r1.p1.y + s*dir1.y = r2.p1.y + t*dir2.y;
			mulFactor = -(dir1.y / dir1.x);
			t = (r1.p1.y + r1.p1.x*mulFactor - r2.p1.y - r2.p1.x*mulFactor) / (dir2.y + dir2.x*mulFactor);
			intersection.p = sf::Vector2f(r2.p1.x + t*dir2.x, r2.p1.y + t*dir2.y);
		}
		else if (dir1.x == 0 && dir2.y == 0) {
			// r1.p1.x                = r2.p1.x + s*dir2.x;
			// r1.p1.y + t*dir1.y = r2.p1.y;
			t = (r2.p1.y - r1.p1.y) / dir1.y;
			intersection.p = sf::Vector2f(r1.p1.x + t*dir1.x, r1.p1.y + t*dir1.y);
		}
		else if (dir2.x == 0 && dir1.y == 0) {
			// r1.p1.x + t*dir1.x = r2.p1.x;
			// r1.p1.y                 = r2.p1.y + s*dir2.y;
			t = (r2.p1.x - r1.p1.x) / dir1.x;
			intersection.p = sf::Vector2f(r1.p1.x + t*dir1.x, r1.p1.y + t*dir1.y);
		}
		else {
			return false;
		}

		if (r1.identify() == RayType::Finite && r2.identify() == RayType::Finite) {
			FiniteRay& fr1 = static_cast<FiniteRay&>(r1);
			FiniteRay& fr2 = static_cast<FiniteRay&>(r2);

			// Limit the result to be within the 2 finite lines bounding boxes
			if (intersection.p.x >= std::min(fr1.p1.x, fr1.p2.x)
				&& intersection.p.x <= std::max(fr1.p1.x, fr1.p2.x)
				&& intersection.p.y >= std::min(fr1.p1.y, fr1.p2.y)
				&& intersection.p.y <= std::max(fr1.p1.y, fr1.p2.y)
				&& intersection.p.x >= std::min(fr2.p1.x, fr2.p2.x)
				&& intersection.p.x <= std::max(fr2.p1.x, fr2.p2.x)
				&& intersection.p.y >= std::min(fr2.p1.y, fr2.p2.y)
				&& intersection.p.y <= std::max(fr2.p1.y, fr2.p2.y)) {

				return true;
			}
			else {
				return false;
			}
		}

		if (r1.identify() == RayType::Infinite && r2.identify() == RayType::Finite) {
			FiniteRay& fr = static_cast<FiniteRay&>(r2);

			// Limit the result to be within the finite lines bounding box
			if (intersection.p.x >= std::min(fr.p1.x, fr.p2.x)
				&& intersection.p.x <= std::max(fr.p1.x, fr.p2.x)
				&& intersection.p.y >= std::min(fr.p1.y, fr.p2.y)
				&& intersection.p.y <= std::max(fr.p1.y, fr.p2.y)) {

				return true;
			}
			else {
				return false;
			}
		}

		if (r1.identify() == RayType::Finite && r2.identify() == RayType::Infinite) {
			FiniteRay& fr = static_cast<FiniteRay&>(r1);

			// Limit the result to be within the finite lines bounding box
			if (intersection.p.x >= std::min(fr.p1.x, fr.p2.x)
				&& intersection.p.x <= std::max(fr.p1.x, fr.p2.x)
				&& intersection.p.y >= std::min(fr.p1.y, fr.p2.y)
				&& intersection.p.y <= std::max(fr.p1.y, fr.p2.y)) {

				return true;
			}
			else {
				return false;
			}
		}

		if (r1.identify() == RayType::Infinite && r2.identify() == RayType::Infinite) {
			return true;
		}
	}

	std::vector<FiniteRay> finiteRays() {
		std::vector<FiniteRay> finiteRays;
		for (auto ray : rays) {
			if (ray->identify() == RayType::Finite) {
				FiniteRay& finiteRay = static_cast<FiniteRay&>(*ray);
				finiteRays.push_back(finiteRay);
			}
		}
		return finiteRays;
	}

	std::vector<InfiniteRay> infiniteRays() {
		std::vector<InfiniteRay> infiniteRays;
		for (auto ray : rays) {
			if (ray->identify() == RayType::Infinite) {
				InfiniteRay& infiniteRay = static_cast<InfiniteRay&>(*ray);
				infiniteRays.push_back(infiniteRay);
			}
		}
		return infiniteRays;
	}


	std::vector<Ray*> rays;
	std::vector<Intersection> intersections;
};



int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Intersection Test");
	window.setVerticalSyncEnabled(true);
	
	FiniteRay currentRay = FiniteRay(sf::Vector2f(0, 0), sf::Vector2f(0, 0));

	RaySystem rs;

	for (float ang = 0; ang < 2 * M_PI; ang += M_PI / 100) {
		rs.spawnRay(new FiniteRay(sf::Vector2f(400 + 5*cos(ang), 300 + 5*sin(ang)), ang, 300));
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Button::Left) {
					currentRay.p1 = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
				}
			}
			if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Button::Left) {
					rs.spawnRay(new FiniteRay(currentRay));
				}
			}
		}		

		window.clear();
			
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			currentRay.p2 = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
			sf::Vertex vertex[] = { currentRay.p1, currentRay.p2 };
			window.draw(vertex, 2, sf::PrimitiveType::Lines);
		}
		rs.renderFiniteLines(window);
		rs.renderInterSectionPoints(window);
		window.display();
	}

	return 0;
}