#pragma once

#include <SFML/Graphics.hpp>
#include "AssetManager.h"

struct Scent;
enum class BirdState
{
	SEARCHING,
	ALERT,
	PURSUING,
	ATTACKING
};

class Bird
{
public:
	Bird(AssetManager& t_assetManager, sf::Vector2f pos);
	void update(float dt);
	void render(sf::RenderWindow& t_window);
	void visionCone();
	void move(float dt);
	void collisionVisionConeScent(std::vector<Scent> t_playerScent);
private:
	sf::Sprite m_body;
	AssetManager &m_assetManager;
	sf::Vector2f m_center;
	float m_radius = 200.0f;
	float m_angle;
	float m_speed = 0.5f;
	BirdState m_state = BirdState::SEARCHING;
	sf::VertexArray m_visionCone;
	float m_angleOfFan = 45.0f;
	float m_reachOfFan = 100.0f;
};

#include "Turtle.h"
