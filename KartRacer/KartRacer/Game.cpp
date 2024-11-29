#include "Game.h"
#include <iostream>
#include "SFML/Audio.hpp"
#include "DrawMap.h"
#include "ClickedCell.h"

// Sets up the display on the screen
Game::Game() :

	// Sets up the window
	m_window{ sf::VideoMode{ 1920U, 1080U, 32U }, "SFML Game" },

	// Game will be over when true
	m_exitGame{ false }
{
	loadMapFromFile("map.txt"); // Load the map from file
}

// Default destructor
Game::~Game()
{
}

// Main game loop
// Update 60 times per second,
// Process update as often as possible and at least 60 times per second

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps);// 60 fps

	// set up the cell sprite's size and color
	cellSprite.setSize(sf::Vector2f(Global::MAP::CELL_SIZE, Global::MAP::CELL_SIZE));
	cellSprite.setFillColor(sf::Color(0, 0, 255));
	cellSprite.setOutlineThickness(1.0f);
	cellSprite.setOutlineColor(sf::Color::Black);

	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}




void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		// Checks if the game is closed
		if (sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		// Checks if a key was pressed
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
		// Checks if a key was released
		if (sf::Event::KeyReleased == newEvent.type)
		{
			keyRelease(newEvent);
		}
	}
}



// Does corresponding action depending on what key was pressed
void Game::processKeys(sf::Event t_event)
{
	// If "escape" was pressed then the game is exited
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
	// Checks if "space" was pressed
	if (sf::Keyboard::Space == t_event.key.code)
	{
	}
	if (sf::Keyboard::S == t_event.key.code)
	{
		saveMapToFile("map.txt"); // Save the map to a file
	}
}

void Game::keyRelease(sf::Event t_event)
{

}

// Update the game world
void Game::update(sf::Time t_deltaTime)
{
	// Checks if the window should be closed
	if (m_exitGame)
	{
		m_window.close();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		player.changeVelocity("Up");
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		player.changeVelocity("Down");
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		player.changeVelocity("Left");
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		player.changeVelocity("Right");
	}

	if (1 == sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || 1 == sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{



		if (mouseClicked == false)
		{
			mouseClicked = true;
		}
		else
		{
			mouseClicked = false;
		}

		if (mouseClicked == true)
		{
			Global::Position<short> cell = getClickedCell(m_window);

			if (0 <= cell.first && 0 <= cell.second && cell.first < Global::MAP::COLUMNS && cell.second < Global::MAP::ROWS)
			{

				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					// makes sure the wall clicked isn't already a wall
					if (Global::MAP::Cell::Wall != map[cell.first][cell.second])
					{
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
						{
							// Mark the cell as a WayPoint
							map[cell.first][cell.second] = Global::MAP::Cell::WayPoint;

							// Add the waypoint to the vector
							sf::Vector2f waypointPosition(
								Global::MAP::X_ORIGIN + cell.first * Global::MAP::CELL_SIZE,
								Global::MAP::Y_ORIGIN + cell.second * Global::MAP::CELL_SIZE
							);

							// Check if the waypoint already exists
							if (std::find(waypoints.begin(), waypoints.end(), waypointPosition) == waypoints.end())
							{
								waypoints.push_back(waypointPosition);
							}

						}
						else
						{
							// makes the cell a wall
							map[cell.first][cell.second] = Global::MAP::Cell::Wall;
						}
					}
				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				{
					if (map[cell.first][cell.second] == Global::MAP::Cell::WayPoint)
					{
						// Convert grid position to screen position
						sf::Vector2f waypointPosition(
							Global::MAP::X_ORIGIN + cell.first * Global::MAP::CELL_SIZE,
							Global::MAP::Y_ORIGIN + cell.second * Global::MAP::CELL_SIZE
						);

						// Remove the waypoint from the vector
						waypoints.erase(std::remove(waypoints.begin(), waypoints.end(), waypointPosition), waypoints.end());
					}

					map[cell.first][cell.second] = Global::MAP::Cell::Empty;
				}

			}
		}
	}

	enemy1.update(player.playerSprite.getPosition(), map, waypoints);
	player.movePlayer();
}

// Renders all sprites and texts
void Game::render()
{
	m_window.clear(sf::Color::White);
	drawMap(m_window, cellSprite, map); 
	enemy1.drawEnemy(m_window);
	player.drawPlayer(m_window);

	m_window.display();
}

void Game::saveMapToFile(const std::string& filename)
{
	std::ofstream outFile(filename);
	if (!outFile.is_open())
	{
		std::cerr << "Error: Could not open file for saving map." << std::endl;
		return;
	}

	for (unsigned short y = 0; y < Global::MAP::ROWS; y++)
	{
		for (unsigned short x = 0; x < Global::MAP::COLUMNS; x++)
		{
			outFile << static_cast<int>(map[x][y]) << " ";
		}
		outFile << "\n";
	}

	// Save waypoints
	outFile << "WAYPOINTS\n";
	for (const auto& waypoint : waypoints)
	{
		outFile << waypoint.x << " " << waypoint.y << "\n";
	}

	outFile.close();
	std::cout << "Map and waypoints saved successfully to " << filename << std::endl;
}

void Game::loadMapFromFile(const std::string& filename)
{
	std::ifstream inFile(filename);
	if (!inFile.is_open())
	{
		std::cerr << "Error: Could not open file for loading map. Using default map." << std::endl;
		return;
	}

	loadedMapFromFile = true;
	waypoints.clear(); // Clear existing waypoints

	for (unsigned short y = 0; y < Global::MAP::ROWS; y++)
	{
		for (unsigned short x = 0; x < Global::MAP::COLUMNS; x++)
		{
			int cellValue;
			if (inFile >> cellValue)
			{
				map[x][y] = static_cast<Global::MAP::Cell>(cellValue);
			}
			else
			{
				map[x][y] = Global::MAP::Empty; // Fallback to empty
			}
		}
	}

    // Read additional waypoints at the end of the file
    std::string line;
    while (std::getline(inFile, line))
    {
        std::istringstream waypointStream(line);
        float x, y;

        if (waypointStream >> x >> y)
        {
            sf::Vector2f explicitWaypoint(x, y);
            waypoints.push_back(explicitWaypoint);
        }
    }

    inFile.close();
    std::cout << "Map and explicit waypoints loaded successfully from " << filename << std::endl;

}
