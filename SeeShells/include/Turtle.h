#pragma once

#include <SFML/Graphics.hpp>

class Turtle
{
private:

	sf::Vector2f m_direction{ 0,0 };

	static int const m_speed{ 100 };

	sf::Texture m_texture{ "./resources/ASSETS/IMAGES/player.png" };
	sf::Sprite m_body{ m_texture };

	void move(float t_dt);

public:

	Turtle();

	//void init();
	void update(float t_dt);

	void render(sf::RenderWindow& t_window);

};

