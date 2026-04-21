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

	sf::Vector2f m_direction{ 0,0 };

	int m_speed{ 100 };

	sf::Sprite m_body;

	AssetManager& m_assetManager;

	void move(float t_dt);

	std::vector<Scent> m_scentTrail;
	sf::Clock m_scentTimer;

	Animation m_animation;
	Animation* m_currAnimation;

public:

	Turtle(AssetManager &t_assetManager);

	//void init();
	void update(float t_dt);

	void render(sf::RenderWindow& t_window);

	void manageScent();
	std::vector<Scent> getScent();
	sf::Sprite getSprite();

};

