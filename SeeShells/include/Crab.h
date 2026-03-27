#pragma once

#include <SFML/Graphics.hpp>
#include "AssetManager.h"


class Crab
{
private:

	bool m_goingLeft{ false };

	int m_speed{ 50 };

	sf::Sprite m_body;

	AssetManager& m_assetManager;

	void move(float t_dt);

	sf::Clock m_moveTimer;
	int m_moveDelay;

public:

	Crab(AssetManager& t_assetManager);

	//void init();
	void update(float t_dt);

	void render(sf::RenderWindow& t_window);

};

