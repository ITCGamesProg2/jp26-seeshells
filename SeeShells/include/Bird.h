#pragma once

#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "Animation.h"
#include "Drawable.h"

struct Scent;
enum class BirdState
{
	SEARCHING,
	ALERT,
	PURSUING,
	ATTACKING
};

class Bird : public Drawable
{
public:
	Bird(AssetManager& t_assetManager, sf::Vector2f pos);
	void update(float dt, std::vector<Scent>& t_playerScent, sf::Sprite& t_turtle, bool t_isHiding);
	void render(sf::RenderWindow& t_window) override;
	void visionCone();
	void move(float dt);
	void collisionVisionConeScent(std::vector<Scent> &t_playerScent);
	sf::Angle lookAt(sf::Vector2f  t_pointToLookAt);
	bool collisionVisionConeTurtle(sf::Sprite& t_turtle);
	bool isPlayerDead();
	
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
	sf::Clock m_killTimer;
	bool m_sawPlayer = false;
	bool m_playerDead = false;
	float m_ignoreScentTimer = 0.0f;
	float m_alertTimer = 0.0f;

	float m_diffUpdating;
	Animation m_moveAnimation;
	Animation* m_currAnimation;

};

#include "Turtle.h"
