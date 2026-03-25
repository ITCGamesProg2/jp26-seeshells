#include "Turtle.h"

Turtle::Turtle()
{
}

void Turtle::update(float t_dt)
{
	move(t_dt);
}

void Turtle::move(float t_dt)
{
	m_direction = { 0,0 };


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		m_direction.y -= 1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		m_direction.x -= 1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		m_direction.y += 1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		m_direction.x += 1;
	}

	if (m_direction.x != 0 && m_direction.y != 0)
	{
		m_direction = m_direction.normalized();
	}
	

	sf::Vector2f pos = m_body.getPosition();
	pos.x += m_direction.x * m_speed * (t_dt/1000);
	pos.y += m_direction.y * m_speed * (t_dt/1000);

	m_body.setPosition(pos);
}

void Turtle::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_body);
}

