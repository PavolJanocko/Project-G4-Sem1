#pragma once
#include <array>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Globals.h"
#include "ClickedCell.h"

Global::Position<short> getClickedCell(const sf::RenderWindow& i_window)
{
	float mouse_x = sf::Mouse::getPosition(i_window).x - Global::MAP::X_ORIGIN;
	float mouse_y = sf::Mouse::getPosition(i_window).y - Global::MAP::Y_ORIGIN;

	// mouse_x and mouse_y are divided by gbl::MAP::CELL_SIZE to convert the position from pixels to grid cell units.
	return Global::Position<short>(floor(mouse_x / Global::MAP::CELL_SIZE), floor(mouse_y / Global::MAP::CELL_SIZE));
}