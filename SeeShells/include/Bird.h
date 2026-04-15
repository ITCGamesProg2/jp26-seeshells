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
	void rotate();
private:
	sf::Sprite m_body;
	AssetManager &m_assetManager;
	sf::Vector2f m_center;
	float m_radius = 200.0f;
	float m_angle;
	float m_speed = 0.5f;
	BirdState m_state = BirdState::SEARCHING;
	sf::VertexArray m_visionCone;
	float m_angleOfFan;
	float m_reachOfFan;
	sf::Vector2f m_chasingPoint;
	sf::Vector2f m_direction;
	sf::Angle m_rotation;
	bool m_rotated = true;
	bool m_moving = true;
};

#include "Turtle.h"
