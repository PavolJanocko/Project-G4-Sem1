#include "Player.h"

Player::Player()
{
	if (!playerTexture.loadFromFile("ASSETS\\IMAGES\\Player.png"))
	{
		// Error message if previous call fails
		std::cout << "problem loading character" << std::endl;
	}
	playerSprite.setTexture(playerTexture);
	playerSprite.setPosition(1920 / 2, 1080 / 2);
	playerSprite.setOrigin(playerSprite.getGlobalBounds().width / 2.0f, playerSprite.getGlobalBounds().height / 2.0f);
	playerSprite.setScale(0.05, 0.05);
}

void Player::movePlayer()
{
	rotationInRadians = playerSprite.getRotation() * 3.14159265 / 180;

	velocity.x = cos(rotationInRadians) * speed;
	velocity.y = sin(rotationInRadians) * speed;
	playerSprite.move(velocity);

	decelerate();
}

void Player::changeVelocity(std::string direction)
{

	if (direction == "Up")
	{
		speed = speed + 0.25;
		if (speed >= 5)
		{
			speed = 5;
		}
	}
	else if (direction == "Down")
	{
		speed = speed - 0.25;
		if (speed <= -5)
		{
			speed = -5;
		}
	}
	else if (direction == "Left")
	{
		playerSprite.rotate(-3.0f);
	}
	else if (direction == "Right")
	{
		playerSprite.rotate(3.0f);
	}
}

void Player::decelerate()
{
	// Apply a small deceleration factor when speed is non-zero
	if (speed > 0)
	{
		speed -= 0.05f; // Decelerate when moving forward
		if (speed < 0)
		{
			speed = 0; // Stop completely when speed reaches zero
		}
	}
	else if (speed < 0)
	{
		speed += 0.05f; // Decelerate when moving backward
		if (speed > 0)
		{
			speed = 0; // Stop completely when speed reaches zero
		}
	}
}

void Player::drawPlayer(sf::RenderWindow& window)
{
	window.draw(playerSprite);
}
