#pragma once

#include <SFML/Graphics.hpp>

class Drawable
{
private:

	sf::Texture m_texture;
	sf::Sprite m_sprite;

public:

	void render(sf::RenderWindow& t_window);
};

