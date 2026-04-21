#pragma once

#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "Animation.h"
#include "Subject.h"

struct Scent
{
	sf::CircleShape m_circle;
	sf::Clock m_timer;
};

class Turtle : public Subject
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

public:

	Turtle(AssetManager &t_assetManager);

	//void init();
	void update(float t_dt);
	void render(sf::RenderWindow& t_window);
	void manageScent();
	std::vector<Scent> getScent();
	sf::Sprite getSprite();
	void handleKeyInput();
	void increaseSpeed();
	void increaseRotation();
	void decreaseSpeed();
	void decreaseRotation();

};

