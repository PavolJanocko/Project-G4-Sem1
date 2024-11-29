#pragma once

#include <SFML/Graphics.hpp>

#include <array>
#include <chrono>
#include <queue>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>


#include "Enemy.h"
#include "Player.h"
#include "Globals.h"

class Game
{
public:
	Game();
	~Game();

	// Main method for game
	void run();

	sf::RectangleShape cellSprite;
	Global::Map<> map;  // main map grid

	void saveMapToFile(const std::string& filename);
	void loadMapFromFile(const std::string& filename);

	std::vector<sf::Vector2f> waypoints;  // Store the positions of waypoint cells

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void keyRelease(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();
	bool loadedMapFromFile = false;


	Enemy enemy1;
	Player player;

	bool mouseClicked = false;  // tracks whether the mouse was clicked
	bool mouse_pressed = 0;
	Global::Position<short> mouse_cell_start;


	sf::RenderWindow m_window; // Main window

	bool m_exitGame; // control exiting game


};