#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include "InputManager.h"
#include "RaySystem.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>



int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Intersection Test");
	window.setVerticalSyncEnabled(true);

	FiniteRay currentRay = FiniteRay(sf::Vector2f(0, 0), sf::Vector2f(0, 0));
	RaySystem rs;
	InputManager inputManager(window);

	for (float ang = 0; ang < 2 * M_PI; ang += M_PI / 100) {
		rs.spawnRay(new FiniteRay(sf::Vector2f(400 + 5*cos(ang), 300 + 5*sin(ang)), ang, 300));
	}

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			inputManager.update(event);

			switch (event.type) {
				case sf::Event::Closed: {
					window.close();
				} break;

				case sf::Event::MouseButtonPressed: {
					if (event.mouseButton.button == sf::Mouse::Button::Left) {
						currentRay.p1 = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
					}
				} break;

				case sf::Event::MouseButtonReleased: {
					if (event.mouseButton.button == sf::Mouse::Button::Left) {
						for (auto ray : rs.finiteRays()) {
							rs.trimEnd(*ray, currentRay);
						}
					}
				} break;
			}
		}

		window.clear();
			
		if (inputManager.isMouseDown(sf::Mouse::Button::Left)) {
			currentRay.p2 = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
			sf::Vertex vertex[] = { currentRay.p1, currentRay.p2 };
			window.draw(vertex, 2, sf::PrimitiveType::Lines);
		}
		rs.renderFiniteRays(window);
		rs.renderInterSectionPoints(window);
		window.display();
	}

	return 0;
}