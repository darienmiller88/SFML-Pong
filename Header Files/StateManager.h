#pragma once
#include <SFML/Graphics.hpp>

class StateManager{
	public:
		StateManager();
		void titleScreen();
		void demo();
		void game();
	private:
		sf::Font font;
};

