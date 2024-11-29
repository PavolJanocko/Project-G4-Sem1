#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Globals.h"

class Player
{

private:

	float speed = 0;
	float rotationInRadians;

public:
	Player();

	sf::Sprite playerSprite;
	sf::Texture playerTexture;

	void movePlayer();
	sf::Vector2f velocity{ 0,0 };
	void changeVelocity(std::string direction);

	void decelerate();

	void drawPlayer(sf::RenderWindow& window);


};