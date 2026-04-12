#pragma once

#include <SFML/Graphics.hpp>
#include "AssetManager.h"

#include <map> 
#include <list>

#define MAX_BRANCHES	10
#define MAX_ROCKS		10
static int const numCols{ 10 };
static int const numRows{ 10 };
static int const cellWidth{ 64 };
static int const cellHeight{ 64 };

class Environment
{
private:

	AssetManager& m_assetManager;

	// Texture
	sf::Texture m_blankTexture{ "resources/IMAGES/temp.png" };

	std::vector<sf::Sprite> m_rocks;
	std::vector<sf::Sprite> m_branches;
	// Need to #include <map> and #include <list>
	// Declaration of std::map
	std::map<int, std::list<sf::Sprite>> spatialMap;

public:

	Environment(AssetManager& t_assetManager);

	/// <summary> Creates the obstacle sprites and loads them into a vector. 
	/// Note that sf::Sprite is considered a light weight class, so 
	/// storing copies (instead of pointers to sf::Sprite) in std::vector 
	/// is acceptable. 
	///  </summary> 
	void generateObstacles();

	bool collision(sf::Sprite t_sprite);


	void render(sf::RenderWindow& t_window);

};

