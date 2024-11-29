#include "Enemy.h"
#include "Globals.h"

Enemy::Enemy()
{
    if (!enemyTexture.loadFromFile("ASSETS\\IMAGES\\Enemy.png"))
    {
        // Error message if previous call fails
        std::cout << "problem loading character" << std::endl;
    }
    enemySprite.setTexture(enemyTexture);
    enemySprite.setPosition(150, 300);
    enemySprite.setOrigin(enemySprite.getGlobalBounds().width / 2.0f, enemySprite.getGlobalBounds().height / 2.0f);
    enemySprite.setScale(0.125, 0.125);

}

void Enemy::update(sf::Vector2f targetPos, const Global::Map<>& map, std::vector<sf::Vector2f> waypoints)
{

    calculateRays();  // Compute ray directions based on enemy rotation
    castRays(map);    // Perform raycasting with those directions
    move(waypoints);
}

void Enemy::move(const std::vector<sf::Vector2f>& waypoints)
{
    if (waypoints.empty())
    {
        return; // No waypoints, so do nothing
    }

    // Get the current waypoint
    sf::Vector2f targetPos = waypoints[currentWaypointIndex];

    // Calculate the direction to the target position
    sf::Vector2f directionToTarget = targetPos - enemySprite.getPosition();
    float distanceToTarget = std::hypot(directionToTarget.x, directionToTarget.y);

    // Check if the enemy is within 20 pixels of the current waypoint
    if (distanceToTarget < 20.0f)
    {
        // Move to the next waypoint
        currentWaypointIndex++;

        // Loop back to the first waypoint if we've reached the last one
        if (currentWaypointIndex >= waypoints.size())
        {
            currentWaypointIndex = 0;
        }

        // Update the target position for the next frame
        targetPos = waypoints[currentWaypointIndex];
        directionToTarget = targetPos - enemySprite.getPosition();
    }

    // Calculate the angle to the target in degrees
    float targetAngle = std::atan2(directionToTarget.y, directionToTarget.x) * 180 / 3.14159265;

    // Get the current rotation of the enemy
    float currentAngle = enemySprite.getRotation();

    // Calculate the shortest rotation direction
    float angleDifference = targetAngle - currentAngle;
    if (angleDifference > 180) angleDifference -= 360;
    if (angleDifference < -180) angleDifference += 360;

 
    if (forwardRay < 100.0f)
    { // If forward ray detects a wall
        if (leftRay > rightRay) {
            angleDifference -= 30; // Steer left
        }
        else {
            angleDifference += 30; // Steer right
        }
    }
    else if (leftRay < 80.0f)
    { // If left ray detects a wall
        angleDifference += 15; // Steer slightly right
    }
    else if (rightRay < 80.0f)
    { // If right ray detects a wall
        angleDifference -= 15; // Steer slightly left
    }

    // Rotate towards the target while avoiding walls
    if (angleDifference > 0)
    {
        enemySprite.rotate(std::min(angleDifference, rotationSpeed));
    }
    else if (angleDifference < 0)
    {
        enemySprite.rotate(std::max(angleDifference, -rotationSpeed));
    }

    // Update the rotation in radians for calculating the velocity
    rotationInRadians = enemySprite.getRotation() * 3.14159265 / 180;

    // Set the velocity based on the updated rotation
    velocity.x = cos(rotationInRadians) * speed;
    velocity.y = sin(rotationInRadians) * speed;

    // Move the enemy sprite in the direction it's facing
    enemySprite.move(velocity);
}

void Enemy::castRays(const Global::Map<>& map)
{

    size_t rayIndex = 0; // Index to track which ray we are processing

    for (auto& direction : rayPaths)
    {
        sf::Vector2f currentPos = enemySprite.getPosition();
        sf::Vector2f normalizedDir = Global::normalize(direction);

        // Small step increment for ray traversal
        sf::Vector2f step = normalizedDir * static_cast<float>(Global::MAP::CELL_SIZE) * 0.1f;

        float traveledDistance = 0.0f;
        float maxDistance = 150.0f;

        while (traveledDistance < maxDistance)
        {
            // Convert position to grid indices
            int gridX = static_cast<int>((currentPos.x - Global::MAP::X_ORIGIN) / Global::MAP::CELL_SIZE);
            int gridY = static_cast<int>((currentPos.y - Global::MAP::Y_ORIGIN) / Global::MAP::CELL_SIZE);

            // Bounds check
            if (gridX < 0 || gridX >= static_cast<int>(Global::MAP::COLUMNS) || gridY < 0 || gridY >= static_cast<int>(Global::MAP::ROWS))
            {
                break; // Ray is out of bounds
            }

            // Check for wall collision
            if (map[gridX][gridY] == Global::MAP::Cell::Wall)
            {
                break; // Ray hit a wall
            }

            // Advance the ray
            currentPos += step;
            traveledDistance += std::hypot(step.x, step.y);
        }

        // Update direction to endpoint
        direction = currentPos;

        // Store the traveled distance in the corresponding variable
        if (rayIndex == 0)
        {
            forwardRay = traveledDistance;
        }
        else if (rayIndex == 1)
        {
            leftRay = traveledDistance;
        }
        else if (rayIndex == 2)
        {
            rightRay = traveledDistance;
        }

        rayIndex++;
    }
}



void Enemy::calculateRays()
{
    rayPaths.clear();

    // Compute the forward vector based on the enemy's rotation
    sf::Vector2f forwardVector = sf::Vector2f(cos(rotationInRadians), sin(rotationInRadians));
    sf::Vector2f leftVector = sf::Vector2f(forwardVector.x * cos(-45 * 3.14159265 / 180) - forwardVector.y * sin(-45 * 3.14159265 / 180), forwardVector.x * sin(-45 * 3.14159265 / 180) + forwardVector.y * cos(-45 * 3.14159265 / 180));
    sf::Vector2f rightVector = sf::Vector2f(forwardVector.x * cos(45 * 3.14159265 / 180) - forwardVector.y * sin(45 * 3.14159265 / 180), forwardVector.x * sin(45 * 3.14159265 / 180) + forwardVector.y * cos(45 * 3.14159265 / 180));
    
    // Define directions for rays
    std::vector<sf::Vector2f> rayDirections = {forwardVector, leftVector, rightVector};

    for (const auto& dir : rayDirections)
    {
        rayPaths.push_back(dir);
    }
}

void Enemy::drawRays(sf::RenderWindow& window)
{
    for (const auto& endpoint : rayPaths)
    {
        sf::Vertex line[] = {sf::Vertex(enemySprite.getPosition(), sf::Color::Red), sf::Vertex(endpoint, sf::Color::Red)};
        window.draw(line, 2, sf::Lines);
    }
}


void Enemy::drawEnemy(sf::RenderWindow& window)
{
    window.draw(enemySprite);
#ifndef NDEBUG
    drawRays(window);
#endif
}