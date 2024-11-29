#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Globals.h"
#include <array>

class Enemy
{
private:
    float rotationInRadians;
    float speed = 3;
    float maxSpeed = 10;
    float rotationSpeed = 3;

    // Variables to store the lengths of each ray
    float forwardRay = 0.0f;
    float leftRay = 0.0f;
    float rightRay = 0.0f;

    std::vector<sf::Vector2f> rayPaths; 

    float sensorLength = 150.0f;
    int currentWaypointIndex = 0;

public:
    Enemy();

    sf::Sprite enemySprite;
    sf::Texture enemyTexture;

    void update(sf::Vector2f targetPos, const Global::Map<>& map, std::vector<sf::Vector2f> waypoints);
    void move(const std::vector<sf::Vector2f>& waypoints);
    void drawEnemy(sf::RenderWindow& window);

    void calculateRays(); // Calculate ray directions
    void castRays(const Global::Map<>& map); // Perform raycasting
    void drawRays(sf::RenderWindow& window);
    sf::Vector2f velocity{ 0,0 };
};
