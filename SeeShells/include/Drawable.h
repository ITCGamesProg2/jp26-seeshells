#pragma once
#include <SFML/Graphics.hpp>


class Drawable
{
public:

	virtual void render(sf::RenderWindow& t_window) = 0;
};

