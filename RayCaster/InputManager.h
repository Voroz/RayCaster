#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <unordered_map>


class InputManager
{
public:
	InputManager(sf::RenderWindow& window);
	~InputManager();

	void update(sf::Event event);
	bool isKeyDown(sf::Keyboard::Key key);
	bool isMouseDown(sf::Mouse::Button button);

private:
	std::unordered_map<sf::Keyboard::Key, bool> _keyMap;
	std::unordered_map<sf::Mouse::Button, bool> _mouseMap;
	sf::RenderWindow& _window;
};

