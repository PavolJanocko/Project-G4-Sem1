#include "Globals.h"

sf::Vector2f Global::normalize(const sf::Vector2f& vector)
{
    float magnitude = std::sqrt(vector.x * vector.x + vector.y * vector.y);
    if (magnitude != 0)
    {
        return vector / magnitude; // Return the normalized vector
    }
    else
    {
        return sf::Vector2f(0.f, 0.f); // Return zero vector if magnitude is zero
    }
}