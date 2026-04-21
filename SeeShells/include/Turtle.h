#pragma once

#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "Animation.h"
#include "Subject.h"
#include "CollisionDetector.h"

struct Scent
{
	sf::CircleShape m_circle;
	sf::Clock m_timer;
};

class Turtle : public Subject
{
private:

	sf::Vector2f m_direction{ 0,0 };

	int m_speed{ 100 };

	sf::Sprite m_body;

	AssetManager& m_assetManager;

	void move(float t_dt);

	std::vector<Scent> m_scentTrail;
	sf::Clock m_scentTimer;

	Animation m_moveAnimation;
	Animation* m_currAnimation;

	enum class CollisionState { NORMAL, COLLIDING };
	CollisionState m_state{ CollisionState::NORMAL };

	std::vector<sf::Sprite>& m_obstacleSprites;
	sf::Vector2f m_contactNormal;

public:

	Turtle(AssetManager &t_assetManager, std::vector<sf::Sprite>& t_obstacleSprites);

	//void init();
	void update(float t_dt);
	void rotate();

	void render(sf::RenderWindow& t_window);

	void manageScent();
	std::vector<Scent> getScent();
	sf::Sprite getSprite();

	bool checkCollision();
	void deflect(float t_dt);

};

