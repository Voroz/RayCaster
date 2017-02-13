#include "InputManager.h"



InputManager::InputManager(sf::RenderWindow& window) :
	_window(window)
{

}

InputManager::~InputManager() {

}

void InputManager::update(sf::Event event) {
	switch (event.type) {
		case sf::Event::KeyPressed: {
			_keyMap[event.key.code] = true;
		} break;

		case sf::Event::KeyReleased: {
			_keyMap[event.key.code] = false;
		} break;

		case sf::Event::MouseButtonPressed: {
			_mouseMap[event.mouseButton.button] = true;
		} break;

		case sf::Event::MouseButtonReleased: {
			_mouseMap[event.mouseButton.button] = false;
		} break;
	}
}

bool InputManager::isKeyDown(sf::Keyboard::Key key) {
	return _keyMap[key];
}

bool InputManager::isMouseDown(sf::Mouse::Button button) {
	return _mouseMap[button];
}