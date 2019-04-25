#include "Paddle.h"

Paddle::Paddle(const sf::Vector2f &position, const sf::Vector2f &dimensions, const sf::Color &paddleColor, 
	const movementKeys &upAndDownKeys, float ySpeed) : upAndDownKeys(upAndDownKeys), Y_SPEED(ySpeed), rect(sf::Vector2f(dimensions)){
	rect.setPosition(position);
	rect.setFillColor(paddleColor);
	score = 0;
}

void Paddle::move(){
	//if the up key is pressed up, move the paddle up
	if (sf::Keyboard::isKeyPressed(upAndDownKeys.first))
		rect.move(0, -Y_SPEED);
	//if the down key is pressed, move the paddle down
	else if (sf::Keyboard::isKeyPressed(upAndDownKeys.second))
		rect.move(0, Y_SPEED);
}

void Paddle::cpuMove(float moveOffSet, float ballYSpeed){
	rect.move(0, ballYSpeed - moveOffSet);
}

void Paddle::setPosition(const sf::Vector2f &position){
	rect.setPosition(position);
}

void Paddle::increaseScore(){
	score++;
}

void Paddle::drawPaddle(sf::RenderWindow &window){
	window.draw(rect);
}

void Paddle::setScore(int newScore){
	score = newScore;
}

sf::Vector2f Paddle::getPosition() const{
	return rect.getPosition();
}

movementKeys Paddle::getUserKeys() const{
	return upAndDownKeys;
}

bool Paddle::intersects(const sf::RectangleShape &ball) const{
	return rect.getGlobalBounds().intersects(sf::FloatRect(sf::Rect<float>( ball.getPosition(), ball.getSize() )));
}

std::string Paddle::getScore() const{
	return std::to_string(score);
}

sf::Vector2f Paddle::getSize() const{
	return rect.getSize();
}

float Paddle::getBottomEdge() const{
	return rect.getPosition().y + rect.getSize().y;
}

float Paddle::getRightEdge() const{
	return rect.getPosition().x + rect.getSize().x;
}

bool Paddle::isBallInRange(const sf::RectangleShape &ball) const{
	return ball.getPosition().y + ball.getSize().y >= rect.getPosition().y and ball.getPosition().y < getBottomEdge();
}