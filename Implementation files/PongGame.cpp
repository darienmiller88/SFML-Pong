#define _USE_MATH_DEFINES

#include "PongGame.h"
#include <cmath>
#include <iostream>

PongGame::PongGame(int windowWidth, int windowHeight, const std::string &title, const Colors &paddleColors, const movementKeys &leftKeys,
const movementKeys &rightKeys) :

//create the window object
window(sf::VideoMode(windowWidth, windowHeight), title, sf::Style::Close | sf::Style::Titlebar), 

//create the leftPaddle object (paddle on the left of the window)
leftPaddle(sf::Vector2f(30, windowHeight / 2), sf::Vector2f(20, 80), paddleColors.first, leftKeys, 10.0f),

//create the rightPaddle object (paddle on the right of the window)
rightPaddle(sf::Vector2f(windowWidth - 20 - 30, windowHeight / 2), sf::Vector2f(20, 80), paddleColors.second, rightKeys, 10.0f), 

ball(sf::Vector2f(15, 15)){
	ball.setPosition(windowWidth / 2, windowHeight / 2);
	setNewDirection();

	if (!font.loadFromFile("C:\\Users\\Darien Miller\\Desktop\\fonts\\times-new-roman.ttf")) {
		std::cout << "file could not be found!";
		exit(1);
	}
}

void PongGame::run(){
	
	window.setFramerateLimit(60);
	while (window.isOpen()) {
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed)
				window.close();
		}   
		 
		window.clear();
		drawLine();
		ball.move(ballVelocity);
		leftPaddle.move();
		rightPaddle.move();
		ballPaddleCollision();
		awardPoint(leftPaddle, rightPaddle);
		constainBall();
		constainPaddles(leftPaddle, rightPaddle);
		leftPaddle.drawPaddle(window);
		rightPaddle.drawPaddle(window);
		printScore(leftPaddle, rightPaddle);
		window.draw(ball);
		window.display();
	}
}

//draws the checkered line in the middle of the pong board
void PongGame::drawLine(){
	//the length of each rectangle, and the distance between each rectangle.
	float rectLen = 15, spacing = 15;

	for (size_t i = 0; i < window.getSize().y; i += (rectLen + spacing)){
		sf::RectangleShape r(sf::Vector2f(3, rectLen));
		r.setPosition(window.getSize().x / 2, i);
		window.draw(r);
	}
}

//make the ball bounce in the opposite direction if it tries to go off the top or bottom of the scree
void PongGame::constainBall(){
	
	//if the ball hits the top or bottom of the window, reverse its velocity int the y direction and cause it to "bounce" off these 
	//boundaries in the opposite direction.
	if (ball.getPosition().y + ball.getSize().y >= window.getSize().y or ball.getPosition().y <= 0)
		ballVelocity.y = -ballVelocity.y;

	//if the ball goes off the window in either direction (right or left side of window), reset the ball to the middle of the screen 
	else if (getBallEdge() >= window.getSize().x or ball.getPosition().x <= 0) {
		ball.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
		setNewDirection();
	}
		
}

//prevent the paddles from leaving the screen by locking it to the top or bottom if the user tries to push it off the screen
void PongGame::constainPaddles(Paddle &leftPaddle, Paddle &rightPaddle){
	//in order to this function to do what is intended, each if statement MUST be tested, as both paddles are moving at the same time
	
	//if either the left or right paddle tries to go above the top of the screen, lock it to the very top.
	if (rightPaddle.getPosition().y <= 0) 
		rightPaddle.setPosition(sf::Vector2f(rightPaddle.getPosition().x, 0));
	if(leftPaddle.getPosition().y <= 0)
		leftPaddle.setPosition(sf::Vector2f(leftPaddle.getPosition().x, 0));

	//Do the same for when either paddle tries to go below the bottom of the screen
	if (leftPaddle.getBottomEdge() > window.getSize().y)
		leftPaddle.setPosition(sf::Vector2f(leftPaddle.getPosition().x, window.getSize().y - leftPaddle.getSize().y));
	if (rightPaddle.getBottomEdge() > window.getSize().y)
		rightPaddle.setPosition(sf::Vector2f(rightPaddle.getPosition().x, window.getSize().y - rightPaddle.getSize().y));
}

//prints the score to the screen
void PongGame::printScore(const Paddle &leftPaddle, const Paddle &rightPaddle) {
	sf::Text textLeft(leftPaddle.getScore(), font, 50);
	sf::Text textRight(rightPaddle.getScore(), font, 50);

	//textLeft.setFillColor(sf::Color::White);
	textLeft.setStyle(sf::Text::Bold);
	textLeft.setPosition(sf::Vector2f((window.getSize().x / 2) - 100, 10));

	//textRight.setFillColor(sf::Color::White);
	textRight.setStyle(sf::Text::Bold);
	textRight.setPosition(sf::Vector2f((window.getSize().x / 2) + 50, 10));

	window.draw(textLeft);
	window.draw(textRight);
}

void PongGame::ballPaddleCollision(){
	if (ball.getPosition().x <= leftPaddle.getRightEdge() and leftPaddle.isBallInRange(ball)) {
		ball.setPosition(sf::Vector2f(leftPaddle.getRightEdge(), ball.getPosition().y));
		ballVelocity.x = -ballVelocity.x;
	}
		
	else if (ball.getPosition().x + ball.getSize().x >= rightPaddle.getPosition().x and rightPaddle.isBallInRange(ball)) {
		ball.setPosition(sf::Vector2f(rightPaddle.getPosition().x - ball.getSize().x, ball.getPosition().y));
		ballVelocity.x = -ballVelocity.x;
	}
}

//Award a point to a player when the ball goes in the other players "goal"
void PongGame::awardPoint(Paddle &leftPaddle, Paddle &rightPaddle){
	if (ball.getPosition().x <= 0)
		rightPaddle.increaseScore();
	else if (getBallEdge() >= window.getSize().x)
		leftPaddle.increaseScore();
}

float PongGame::getBallEdge() const{
	return ball.getPosition().x + ball.getSize().x;
}

//Determine the direction of the ball's trajectory through the use of angles.
void PongGame::setNewDirection() {
	//magnitude of the "ballVelocity" vector.
	int magnitude = 12, angle = chooseProperAngle(magnitude);

	//in order to find the x and y components of a vector, apply the following formula.
	ballVelocity.x = magnitude * cos((angle * M_PI) / 180);
	ballVelocity.y = magnitude * sin((angle * M_PI) / 180);

	std::cout << "x: " << ballVelocity.x << "\n";
	std::cout << "y: " << ballVelocity.y << "\n\n";
}


/*
* This function is necessary due to that fact that there are four angles that will result in the ball going perfectly horizontally
* or vertically (0 (right), 90 (up), 180 (left), 270). In order to ensure that the ball will always move diagonally, we need to re-roll
* if we obtain one of these angles.
*/
int PongGame::chooseProperAngle(int magnitude){
	int angle = rand.getIntInRange(0, 180);

	//in order to prevent generating angles that are too steep, we will re -roll for a new angle if we get an angle such that 
	//magnitude * sin(angle) is less than magnitude / 2 or magnitude * cos(angle) is less than 1. This will eliminate steep angle 
	//values as when the x component of velocity is low, the ball move vertically far faster than it will move horizontally,
	//increasing the amount of time the ball will take to reach either paddle.
	while (abs((magnitude * cos((angle * M_PI) / 180))) < magnitude / 2 or abs((magnitude * sin((angle * M_PI) / 180))) < 1)
		angle = rand.getIntInRange(0, 180);
	return angle;
}
