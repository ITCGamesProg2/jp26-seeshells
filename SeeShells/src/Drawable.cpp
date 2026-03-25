#include "Drawable.h"

void Drawable::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_sprite);
}
