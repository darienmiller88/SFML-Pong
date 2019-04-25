#pragma once
#include "Paddle.h"
#include "Random.h"
#include <unordered_map>
#include <SFML/Audio.hpp>

//typedef to simplify this long winded declaration. The need for a pair of sf::Color is to allow the user to define what color each 
//paddle will be.
using Colors = std::pair<sf::Color, sf::Color>;

/*
* PongGame class to control to the game logic and piece together the building blocks of the Pong
*/
class PongGame{
	public:

		//The user must pass in the size of the window, as well as the title, but they can also optionally pass in the controls for each 
		//paddle and a pair of color srepresenting the color of each paddle.  
		PongGame(int windowWidth, int windowHeight, const std::string& title, 
			const Colors &paddleColor = Colors(sf::Color::White, sf::Color::White),
			const movementKeys &leftKeys = movementKeys(sf::Keyboard::A, sf::Keyboard::Z),
			const movementKeys &rightKeys = movementKeys(sf::Keyboard::K, sf::Keyboard::M)
		);
		void run();

	//functions for gameplay
	private:
		void pauseGame();
		void resetGame();
		void pauseBall();
		void winGame();
		void manageGameStates();
		void demo();
		void playerVsPlayerOrComputer();
		void cpuTracking(Paddle &cpuPaddle);
		void manageGame();
		int chooseProperAngle(int magnitude);
		void constainBall();
		void constainPaddles(Paddle &leftPaddle, Paddle &rightPaddle);
		void setNewDirection();
		void ballPaddleCollision(const Paddle &leftPaddle, const Paddle &rightPaddle);

	//functions for drawing/non gameplay utilities
	private:
		sf::Text createText(const std::string &word, int textSize, const sf::Text::Style &style, const sf::Vector2u &position);
		void createTitleAssets();
		void printScore(const Paddle &leftPaddle, const Paddle &rightPaddle);
		void drawTitleScreen();
		bool isTextClicked(const sf::Text &text);
		void drawLine();
		void drawInstructionsScreen();
	private:
		sf::RenderWindow window;
		sf::RectangleShape ball;
		sf::Vector2f ballVelocity;
		sf::Font font;
		sf::Text leftPaddleScore, rightPaddleScore;
		sf::SoundBuffer paddleBuffer, ballBuffer;
		sf::Sound paddleSound, ballSound;
		sf::Clock clock;
		const std::string MAX_SCORE;
		Paddle leftPaddle, rightPaddle;
		Random rand;
		enum class gameStates { TITLE_SCREEN, INSTRUCTIONS, SINGLE_PLAYER, TWO_PLAYER, DEMO };
		bool isOnePlayer, isTwoPlayer;
		gameStates currentState;
		std::unordered_map<gameStates, sf::Text> titleAssets;
};

