#pragma once
#include "Paddle.h"
#include "Random.h"

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
	private:
		void drawLine();
		int chooseProperAngle(int magnitude);
		void constainBall();
		void constainPaddles(Paddle &leftPaddle, Paddle &rightPaddle);
		void setNewDirection();
		void printScore(const Paddle &leftPaddle, const Paddle &rightPaddle);
		void ballPaddleCollision();
		void awardPoint(Paddle &leftPaddle, Paddle &rightPaddle);
		float getBallEdge() const;
	private:
		sf::RenderWindow window;
		sf::RectangleShape ball;
		sf::Vector2f ballVelocity;
		Paddle leftPaddle, rightPaddle;
		Random rand;
		sf::Font font;
};

