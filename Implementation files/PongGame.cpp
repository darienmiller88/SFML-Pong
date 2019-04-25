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

//create a 15 x 15 "ball", initialize our StateManager object with the window size, as well as the score texts for both paddles.
ball(sf::Vector2f(15, 15)), leftPaddleScore("", font, 50), rightPaddleScore("", font, 50), currentState(gameStates::TITLE_SCREEN),

MAX_SCORE("5"), isOnePlayer(false), isTwoPlayer(false){
	ball.setPosition(windowWidth / 2, windowHeight / 2);
	setNewDirection();

	//set the text of the left paddle score to bold, and it's position to be to the left of the checkered line in the middle.
	leftPaddleScore.setStyle(sf::Text::Bold);
	leftPaddleScore.setPosition(sf::Vector2f((window.getSize().x / 2) - 90, 10));

	//set the text of the right paddle score to bold, and it's position to be to the right of the checkered line in the middle.
	rightPaddleScore.setStyle(sf::Text::Bold);
	rightPaddleScore.setPosition(sf::Vector2f((window.getSize().x / 2) + 50, 10));
	
	
	if (!font.loadFromFile("C:\\Users\\Darien Miller\\Desktop\\fonts\\times-new-roman.ttf")) 
		std::cout << "font file could not be found!";
	if(!ballBuffer.loadFromFile("C:\\Users\\Darien Miller\\Desktop\\pongSoundEffects\\ball.wav"))
		std::cout << "ball sound file could not be found!";
	if(!paddleBuffer.loadFromFile("C:\\Users\\Darien Miller\\Desktop\\pongSoundEffects\\paddle.wav"))
		std::cout << "paddle sound file could not be found!";

	paddleSound.setBuffer(paddleBuffer);
	ballSound.setBuffer(ballBuffer);
	createTitleAssets();
}

void PongGame::run(){
	manageGame();
}

//reset both scores to 0, set the ball back to the middle, and give the ball a new direction to go towards.
void PongGame::resetGame(){
	leftPaddle.setScore(0);
	rightPaddle.setScore(0);
	ball.setFillColor(sf::Color::White);
	ball.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
	setNewDirection();
}

//cause the ball to "disappear" from view and prevent it from moving
void PongGame::pauseBall(){
	ballVelocity.x = ballVelocity.y = 0;
	ball.setFillColor(sf::Color::Black);
	ball.setPosition(window.getSize().x / 2 - ball.getSize().x, window.getSize().y / 2);
}

void PongGame::winGame(){
	sf::Text winText, reset, playAgain;

	if (rightPaddle.getScore() == MAX_SCORE) {
		pauseBall();
		ballSound.stop();
		winText = createText("WIN", 110, sf::Text::Bold, sf::Vector2u(window.getSize().x / 2 + 40, window.getSize().y / 2 - 110));
		reset = createText("-RESET", 40, sf::Text::Bold,
			sf::Vector2u(leftPaddle.getRightEdge() + 30, winText.getPosition().y + winText.getGlobalBounds().height + 50));
		playAgain = createText("-PLAY AGAIN", 40, sf::Text::Bold,
			sf::Vector2u(leftPaddle.getRightEdge() + 30, reset.getPosition().y + reset.getGlobalBounds().height + 50));
	}
	else if (leftPaddle.getScore() == MAX_SCORE) {
		pauseBall();
		ballSound.stop();
		winText = createText("WIN", 110, sf::Text::Bold, sf::Vector2u(leftPaddle.getRightEdge() + 40, window.getSize().y / 2 - 110));
		reset = createText("-RESET", 40, sf::Text::Bold,
			sf::Vector2u(leftPaddle.getRightEdge() + 30, winText.getPosition().y + winText.getGlobalBounds().height + 50));
		playAgain = createText("-PLAY AGAIN", 40, sf::Text::Bold,
			sf::Vector2u(leftPaddle.getRightEdge() + 30, reset.getPosition().y + reset.getGlobalBounds().height + 50));
	}

	window.draw(winText);
	window.draw(reset);
	window.draw(playAgain);

	if (isTextClicked(reset) and (leftPaddle.getScore() == MAX_SCORE or rightPaddle.getScore() == MAX_SCORE)) {
		currentState = gameStates::TITLE_SCREEN;
		resetGame();
	}else if (isTextClicked(playAgain) and (leftPaddle.getScore() == MAX_SCORE or rightPaddle.getScore() == MAX_SCORE)) {
		resetGame();
	}
}

void PongGame::manageGameStates(){
	if (isTextClicked(titleAssets.find(gameStates::SINGLE_PLAYER)->second)) {
		currentState = gameStates::INSTRUCTIONS;
		isOnePlayer = true;
		isTwoPlayer = false;
	}else if (isTextClicked(titleAssets.find(gameStates::TWO_PLAYER)->second)) {
		currentState = gameStates::INSTRUCTIONS;
		isTwoPlayer = true;
		isOnePlayer = false;
	}else if (isTextClicked(titleAssets.find(gameStates::DEMO)->second))
		currentState = gameStates::DEMO;
}

//Function to allow two computers to play against each other 
void PongGame::demo(){

	//move the ball lol
	ball.move(ballVelocity);

	//allow both computer controlled paddles to track the movement of the ball's y position and move accordingly
	cpuTracking(leftPaddle);
	cpuTracking(rightPaddle);

	//allow the ball to bounce off of the paddles.
	ballPaddleCollision(leftPaddle, rightPaddle);

	//prevent the ball from leaving the screen
	constainBall();

	//prevent the paddles from leaving the screen
	constainPaddles(leftPaddle, rightPaddle);

	printScore(leftPaddle, rightPaddle);
	
	sf::Text returnToTitle("- QUIT DEMO MODE", font, 35);
	returnToTitle.setPosition(sf::Vector2f(window.getSize().x / 4, window.getSize().y - returnToTitle.getGlobalBounds().height - 50));

	//draw both paddles to the screeen.
	leftPaddle.drawPaddle(window);
	rightPaddle.drawPaddle(window);
	
	drawLine();
	window.draw(ball);
	window.draw(returnToTitle);
	
	//if the text is clicked, set the current game state to the title screen
	if (isTextClicked(returnToTitle)) {
		leftPaddle.setScore(0);
		rightPaddle.setScore(0);
		currentState = gameStates::TITLE_SCREEN;
	}
}

//function to allow the either two players or a player and a computer to play against each other
void PongGame::playerVsPlayerOrComputer(){
	//if the user presses the escape key, reset the score of both paddles to 0 and change the game state to the title screen.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		leftPaddle.setScore(0);
		rightPaddle.setScore(0);
		currentState = gameStates::TITLE_SCREEN;
	}

	ball.move(ballVelocity);
	leftPaddle.move();

	//if the current gamestate is single player, allow the paddle to move by itself. Otherwise, player two moves the right paddle
	(currentState == gameStates::SINGLE_PLAYER) ? cpuTracking(rightPaddle) : rightPaddle.move(); 
	ballPaddleCollision(leftPaddle, rightPaddle);
	constainBall();
	constainPaddles(leftPaddle, rightPaddle);
	printScore(leftPaddle, rightPaddle);
	drawLine();
	leftPaddle.drawPaddle(window);
	rightPaddle.drawPaddle(window);
	clock.restart();//make sure the clock is restarted to ensure the elapsed time doesn't accumulate while the game is running.
	winGame();
	window.draw(ball);

}

void PongGame::cpuTracking(Paddle &cpuPaddle) {
	//in order to prevent the computer from being unbeatable, we will add an offset to slow down the cpu's ability to perfectly
	//adjust itself to the balls movement. That offset will be the balls y velocity - 1.

	//if the ball is below the the paddle, move the paddle down.
	if (cpuPaddle.getBottomEdge() < ball.getPosition().y)
		cpuPaddle.cpuMove(0, abs(ballVelocity.y) - 1);

	//otherwise, move the paddle up.
	else //if (cpuPaddle.getPosition().y > ball.getPosition().y)
		cpuPaddle.cpuMove(0, -abs(ballVelocity.y) - 1);


	
}

void PongGame::manageGame(){
	window.setFramerateLimit(60);
	
	sf::Time start;
	while (window.isOpen()) {
		
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		switch (currentState){
			case gameStates::TITLE_SCREEN:	
				drawTitleScreen();
				//only get the elapsed time when one the title screen. This will prevent the time from accumulating when on the demo screen
				start = clock.getElapsedTime();
				break;
			case gameStates::DEMO:
				demo();
				break;
			case gameStates::SINGLE_PLAYER:
			case gameStates::TWO_PLAYER:
				playerVsPlayerOrComputer();

				break;
			case gameStates::INSTRUCTIONS:
				drawInstructionsScreen();
		}
		
		//After ten seconds, change the gamestate to the "demo" and play the demo
		if (start.asSeconds() >= 10.0f) {
			currentState = gameStates::DEMO;
			clock.restart();
			start = start.Zero;
		}
			
		manageGameStates();
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

void PongGame::drawInstructionsScreen(){
	sf::Text title(createText("PONG INSTRUCTIONS", 40, sf::Text::Bold, sf::Vector2u(window.getSize().x / 4, 20)));
	sf::Text playerOneText(createText("PLAYER ONE CONTROLS THE LEFT PADDLE WITH THE\n DEFAULT CONTROLS: A = MOVE UP Z = MOVE DOWN", 20,
		sf::Text::Bold, sf::Vector2u(0, title.getPosition().y + title.getGlobalBounds().height + 20)));
	sf::Text playerTwoText(createText("PLAYER TWO CONTROLS THE RIGHT PADDLE WITH THE\n DEFAULT CONTROLS: K = MOVE UP M = MOVE DOWN",
		20, sf::Text::Bold, sf::Vector2u(0, playerOneText.getPosition().y + playerOneText.getGlobalBounds().height + 80)));
	sf::Text exitGame(createText("EXIT TO MENU = ESC", 20, sf::Text::Bold, sf::Vector2u(0, playerTwoText.getPosition().y + 
		playerTwoText.getGlobalBounds().height + 80)));
	sf::Text goToGame(createText("PRESS SPACE TO CONTINUE", 20, sf::Text::Bold, sf::Vector2u(0, exitGame.getPosition().y +
		exitGame.getGlobalBounds().height + 80)));

	window.draw(title);
	window.draw(playerOneText);
	window.draw(playerTwoText);
	window.draw(exitGame);
	window.draw(goToGame);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) and isOnePlayer)
		currentState = gameStates::SINGLE_PLAYER;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) and isTwoPlayer)
		currentState = gameStates::TWO_PLAYER;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		currentState = gameStates::TITLE_SCREEN;
}

//make the ball bounce in the opposite direction if it tries to go off the top or bottom of the screen.
void PongGame::constainBall(){
	
	//if the ball hits the top or bottom of the window, reverse its velocity int the y direction and cause it to "bounce" off these 
	//boundaries in the opposite direction.
	if (ball.getPosition().y + ball.getSize().y >= window.getSize().y or ball.getPosition().y <= 0) {
		ballVelocity.y = -ballVelocity.y;
		ballSound.play();
	}
		

	//if the ball goes off the window in either direction (right or left side of window), reset the ball to the middle of the screen 
	else if (ball.getSize().x + ball.getPosition().x  >= window.getSize().x) {
		ball.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
		setNewDirection();
		leftPaddle.increaseScore();
	} else if(ball.getPosition().x <= 0) {
		ball.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
		setNewDirection();
		rightPaddle.increaseScore();
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
	leftPaddleScore.setString(leftPaddle.getScore());
	rightPaddleScore.setString(rightPaddle.getScore());

	window.draw(leftPaddleScore);
	window.draw(rightPaddleScore);
}

//Handle the collision between the ball and the paddle
void PongGame::ballPaddleCollision(const Paddle &leftPaddle, const Paddle &rightPaddle){
	
	if (leftPaddle.intersects(ball)) {
		ball.setPosition(sf::Vector2f(leftPaddle.getRightEdge(), ball.getPosition().y));
		ballVelocity.x = -ballVelocity.x;
		paddleSound.play();
	}
		
	else if (rightPaddle.intersects(ball)) {
		ball.setPosition(sf::Vector2f(rightPaddle.getPosition().x - ball.getSize().x, ball.getPosition().y));
		ballVelocity.x = -ballVelocity.x;
		paddleSound.play();
	}
}

sf::Text PongGame::createText(const std::string& word, int textSize, const sf::Text::Style& style, const sf::Vector2u& position){
	sf::Text text(word, font, textSize);

	text.setStyle(style), text.setPosition((sf::Vector2f)position);
	return text;
}

void PongGame::createTitleAssets(){
	titleAssets[gameStates::SINGLE_PLAYER] = createText("1 PLAYER", 40, sf::Text::Bold, sf::Vector2u(window.getSize().x / 3, 200));
	titleAssets[gameStates::TWO_PLAYER] = createText("2 PLAYER", 40, sf::Text::Bold, sf::Vector2u(window.getSize().x / 3, 300));
	titleAssets[gameStates::DEMO] = createText("DEMO", 40, sf::Text::Bold, sf::Vector2u(window.getSize().x / 3, 400));
	//titleAssets[gameStates::SETTINGS] = createText("SETTINGS", 40, sf::Text::Bold, sf::Vector2u(window.getSize().x / 3, 500));
}

void PongGame::drawTitleScreen(){
	sf::Text title = createText("PONG", 90, sf::Text::Bold, sf::Vector2u(window.getSize().x / 3.5, 20));

	window.draw(title);
	window.draw(createText("CLICK AN OPTION!!!", 25, sf::Text::Bold, sf::Vector2u(window.getSize().x / 3.5, title.getPosition().y +
		title.getGlobalBounds().height * 1.5)));
	for (auto &asset : titleAssets) {
		//create a rectangle that is the same height and width as the text height.
		sf::RectangleShape rect(sf::Vector2f(asset.second.getLocalBounds().height, asset.second.getLocalBounds().height));
		
		//set the position of the rectangle to be to the left of the text, which will make it resemble a bullet point.
		rect.setPosition(sf::Vector2f(asset.second.getPosition().x - rect.getSize().x * 2, asset.second.getPosition().y + 10));
		window.draw(rect);
		window.draw(asset.second);
	}
		
}

bool PongGame::isTextClicked(const sf::Text &text){
	//if the user clicks on the left mouse button while the mouse cursor is hovering over the text, return true and let the caller
	//function handle the click event.
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) and 
		text.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
		std::cout << "button is clicked\n";
		return true;
	}
	//std::cout << "button is not clicked\n";
	return false;
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
