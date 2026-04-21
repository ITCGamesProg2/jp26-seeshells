#pragma once

#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "Animation.h"


class Crab
{
private:

	static int const numCols{ 10 };
	static int const numRows{ 10 };
	static int const cellWidth{ 64 };
	static int const cellHeight{ 64 };

	bool m_goingLeft{ false };

	int m_speed{ 50 };

	sf::Sprite m_body;

	AssetManager& m_assetManager;

	void move(float t_dt);

	sf::Clock m_moveTimer;
	int m_moveDelay;

	Animation m_moveAnimation;
	Animation* m_currAnimation;

public:

	Crab(AssetManager& t_assetManager, std::map<int, std::list<sf::Sprite>>& t_spatialMap);

	void update(float t_dt);

	void updateSpatialMap(std::map<int, std::list<sf::Sprite>>& t_spatialMap);

	void render(sf::RenderWindow& t_window);

	sf::Sprite getSprite();
};

