#pragma once

#include <SFML/Graphics.hpp>
#include "AssetManager.h"

class Bird
{
public:
	Bird(AssetManager& t_assetManager, sf::Vector2f pos);
	void update(float dt);
	void render(sf::RenderWindow& t_window);
	sf::VertexArray visionCone();
	void move(float dt);
private:
	sf::Sprite m_body;
	AssetManager &m_assetManager;
	sf::Vector2f m_center;
	float m_radius = 200.0f;
	float m_angle;
	float m_speed = 0.5f;
};

