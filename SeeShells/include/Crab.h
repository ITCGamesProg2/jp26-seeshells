#pragma once

#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "Animation.h"
#include "Drawable.h"

class Crab : public Drawable
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

	sf::Vector2f m_position;

public:

	Crab(AssetManager& t_assetManager, std::map<int, std::list<sf::Sprite>>& t_spatialMap, sf::Vector2f t_pos);


	void update(float t_dt);
	/// <summary>
	/// Swap the direction the crab is moving
	/// </summary>
	void changeDirection();

	/// <summary>
	/// Remove the old values of crab from the spatial grid 
	/// and add values for current position
	/// </summary>
	/// <param name="t_spatialMap"></param>
	void updateSpatialMap(std::map<int, std::list<sf::Sprite>>& t_spatialMap);

	void render(sf::RenderWindow& t_window) override;

	sf::Sprite getSprite();
};

