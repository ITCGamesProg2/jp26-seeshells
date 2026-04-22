#pragma once

#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "Animation.h"
#include "Subject.h"
#include "CollisionDetector.h"
#include "Drawable.h"

struct Scent
{
	sf::CircleShape m_circle;
	sf::Clock m_timer;
};

class Turtle : public Subject, public Drawable
{
private:

	float m_speed{ 0.0f };

	sf::Sprite m_body;

	AssetManager& m_assetManager;

	std::vector<Scent> m_scentTrail;
	sf::Clock m_scentTimer;

	Animation m_moveAnimation;
	Animation* m_currAnimation;

	sf::Angle m_rotation{sf::degrees(0.0f)};

	bool m_isHiding = false;

	void move(float t_dt);

public:

	Turtle(AssetManager &t_assetManager, std::vector<sf::Sprite>& t_obstacleSprites);
	enum class CollisionState { NORMAL, COLLIDING };
	CollisionState m_state{ CollisionState::NORMAL };

	std::vector<sf::Sprite>& m_obstacleSprites;
	sf::Vector2f m_contactNormal;
	//void init();

	void update(float t_dt);

	void render(sf::RenderWindow& t_window) override;
	void manageScent();
	std::vector<Scent> getScent();
	sf::Sprite getSprite();
	void handleKeyInput();
	void increaseSpeed();
	void increaseRotation();
	void decreaseSpeed();
	void decreaseRotation();
	bool isPlayerHiding();

	bool checkCollision();
	void deflect(float t_dt);

};

