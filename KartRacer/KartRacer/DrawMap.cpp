#include <array>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Globals.h"
#include "DrawMap.h"

void drawMap(sf::RenderWindow& m_window, sf::RectangleShape& cellSprite, const Global::Map<>& map)
{

	for (unsigned short a = 0; a < Global::MAP::COLUMNS; a++)
	{
		for (unsigned short b = 0; b < Global::MAP::ROWS; b++)
		{
			cellSprite.setPosition(Global::MAP::X_ORIGIN + a * Global::MAP::CELL_SIZE, Global::MAP::Y_ORIGIN + b * Global::MAP::CELL_SIZE);

			Global::MAP::Cell cell_type = map[a][b];

			switch (cell_type)
			{
			case Global::MAP::Cell::Empty:
			{
				cellSprite.setFillColor(sf::Color(36, 36, 85));
				break;
			}
			case Global::MAP::Cell::Wall:
			{
				cellSprite.setFillColor(sf::Color(255, 255, 255));
				break;
			}
			case Global::MAP::Cell::WayPoint:
			{
				cellSprite.setFillColor(sf::Color(0, 0, 255));
			}
			
		}

			m_window.draw(cellSprite);

		}
	}
}