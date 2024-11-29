#pragma once
#include <SFML/Graphics.hpp>


namespace Global
{
    // Normalize a vector
    sf::Vector2f normalize(const sf::Vector2f& vector);

	namespace MAP
	{
		constexpr unsigned char CELL_SIZE = 20;
		constexpr unsigned int COLUMNS = 95;
		constexpr unsigned int ROWS = 50;

		constexpr unsigned char X_ORIGIN = 10;
		constexpr unsigned char Y_ORIGIN = 10;


		enum Cell
		{
			Empty,
			Wall,
			WayPoint
		};
	}


	// type alias for a 2D array (map) with customizable cell types, defaulting to MAP::Cell.
	template <typename value_type = MAP::Cell>
	using Map = std::array<std::array<value_type, MAP::ROWS>, MAP::COLUMNS>;

	// type alias for a position represented as a pair of values (x, y ),
	// with a default type of 'unsigned short'.
	// this is used for positions in the map.
	template <typename value_type = unsigned short>
	using Position = std::pair<value_type, value_type>;
}