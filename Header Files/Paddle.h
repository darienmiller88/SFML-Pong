#pragma once
#include <SFML/Graphics.hpp>
#include <string>

//typedef to simplify this long winded declaration. Below, the need for a pair of keyboard keys is to allow the user to define what key
//will move the paddle up and down.
using movementKeys = std::pair<sf::Keyboard::Key, sf::Keyboard::Key>;

/*
* class to determine the behavior of a pong paddle and store all of the relevant information.
*/
class Paddle{
	public:
		//constructor will take in the initial position of the paddle, the dimensions (how wide and long) of the paddle, its color, a
		//"movementKeys" pair that will contain the keys that will control the paddle, and how fast the paddle moves up and down.
		Paddle(const sf::Vector2f &position, const sf::Vector2f &dimensions, const sf::Color &paddleColor, 
			const movementKeys &upAndDownKeys, float ySpeed);

		//function that allows the user to move their paddle by pressing the keys they assigned to it.
		void move();

		//include a function that will allows a paddle to move by itself, in accordance with the ball's y position subtracted by an
		//"offset" that prevents the paddle from being perfect.
		void cpuMove(float moveOffSet, float ballYSpeed);
		void setPosition(const sf::Vector2f &position);
		void increaseScore();
		void drawPaddle(sf::RenderWindow &window);
		void setScore(int newScore);
		sf::Vector2f getPosition() const;
		std::string getScore() const;
		sf::Vector2f getSize() const;
		movementKeys getUserKeys() const;

		bool intersects(const sf::RectangleShape& ball) const;
		
		//return the y position of the bottom of the paddle
		float getBottomEdge() const;

		//return the x position of the right side of the paddle. We don't have to write one for the left because we get it for free due
		//to the default (x, y) position of the rect object holding it.
		float getRightEdge() const;

		//checks the ball to see if it is between the top and bottom of the paddle
		bool isBallInRange(const sf::RectangleShape &ball) const;

	private:

		//a pair of SFML key representing which keyboard key moves the paddle up and down. first = up key. second = down key.
		movementKeys upAndDownKeys;

		//the rectangle representing the shape of the paddle
		sf::RectangleShape rect;

		//how fast the paddle moves up and down
		const float Y_SPEED;

		int score;
};

