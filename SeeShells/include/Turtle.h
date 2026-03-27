#pragma once

#include <SFML/Graphics.hpp>
#include "AssetManager.h"

class Turtle
{
private:

	sf::Vector2f m_direction{ 0,0 };

	int m_speed{ 100 };

	sf::Sprite m_body;

	AssetManager& m_assetManager;

	void move(float t_dt);

public:

	Turtle(AssetManager &t_assetManager);

	//void init();
	void update(float t_dt);

	void render(sf::RenderWindow& t_window);

};

