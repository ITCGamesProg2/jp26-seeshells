#pragma once

#include <SFML/Graphics.hpp>
#include "AssetManager.h"

#include <map> 
#include <list>

#define MAX_OBSTACLES	20



class Environment
{
private:

	static int const numCols{ 10 };
	static int const numRows{ 10 };
	static int const cellWidth{ 64 };
	static int const cellHeight{ 64 };

	AssetManager& m_assetManager;

	// Need to #include <map> and #include <list>
	// Declaration of std::map
	std::map<int, std::list<sf::Sprite>> m_spatialMap;

	std::vector<sf::Sprite> m_rocks;
	std::vector<sf::Sprite> m_branches;

public:

	Environment(AssetManager& t_assetManager);

	/// <summary> Creates the obstacle sprites and loads them into a vector. 
	/// Note that sf::Sprite is considered a light weight class, so 
	/// storing copies (instead of pointers to sf::Sprite) in std::vector 
	/// is acceptable. 
	///  </summary> 
	void generateObstacles();


	/// <summary>
	/// Function that checks which tile the t_entity is in, then checks if any obstacles in that same tile.
	/// If there is, then collision is checked only with those obstalces. Each corner is checked for the
	/// respective tile location due to the possibility that t_entity could span multiple tiles.
	/// </summary>
	/// <param name="t_sprite"></param>
	/// <returns></returns>
	bool entityCollison(sf::Sprite t_entity);


	void render(sf::RenderWindow& t_window);

};

