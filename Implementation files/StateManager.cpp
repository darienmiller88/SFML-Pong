#include "StateManager.h"
#include <iostream>

StateManager::StateManager(){

	if (!font.loadFromFile("C:\\Users\\Darien Miller\\Desktop\\fonts\\times-new-roman.ttf")) {
		std::cout << "file could not be found!";
		exit(1);
	}

}

void StateManager::titleScreen(sf::RenderWindow &window){
	sf::Text title("PONG", font, 90), singlePlayer("1 PLAYER", font, 30), multiPlayer("2 PLAYER", font, 30), demo("DEMO", font, 30);
	sf::Text settings("SETTINGS", font, 30);

	title.setStyle(title.Bold);
	title.setPosition(sf::Vector2f(window.getSize().x / 2, 40));

	singlePlayer.setStyle(singlePlayer.Bold);
	singlePlayer.setPosition(sf::Vector2f(window.getSize().x / 2, 60));

	multiPlayer.setStyle(multiPlayer.Bold);
	multiPlayer.setStyle

	demo.setStyle(demo.Bold);
	settings.setStyle(settings.Bold);



}

void StateManager::demo(){

}

void StateManager::game(){

}