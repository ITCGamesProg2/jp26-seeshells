#pragma once

#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "Animation.h"

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
	void update(float dt, std::vector<Scent> &t_playerScent);
	void render(sf::RenderWindow& t_window);
	void visionCone();
	void move(float dt);
	void collisionVisionConeScent(std::vector<Scent> &t_playerScent);
	sf::Angle lookAt(sf::Vector2f  t_pointToLookAt);
	
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
	sf::Vector2f m_movingFrom;
	bool m_chasing = false;
	bool m_returning = false;

	float m_diffUpdating;
	Animation m_animation;
};

#include "Turtle.h"
