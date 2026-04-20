#include "Turtle.h"

Turtle::Turtle(AssetManager &t_assetManager)
	: m_assetManager(t_assetManager), 
	m_body(t_assetManager.getTexture("player"))
{
	m_body.setOrigin({ 16.0f,16.0f }); 
}

void Turtle::update(float t_dt)
{
	if (m_scentTrail.size() == 0.0f)
	{
		manageScent();
	}
	if (static_cast<float>(m_scentTimer.getElapsedTime().asSeconds()) >= 1.0f)
	{
		m_scentTimer.reset();
		manageScent();

	}
	move(t_dt);

	for (int i = 0; i < m_scentTrail.size(); i++)
	{
		if (static_cast<float>(m_scentTrail.at(i).m_timer.getElapsedTime().asSeconds()) >= 4.0f)
		{
			Scent temp = m_scentTrail.at(m_scentTrail.size() - 1);
			m_scentTrail.at(i) = temp;
			m_scentTrail.pop_back();
		}
	}
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
	for (int i = 0; i < m_scentTrail.size(); i++)
	{
		t_window.draw(m_scentTrail.at(i).m_circle);
	}
}

void Turtle::manageScent()
{
	Scent scent;
	sf::CircleShape circle;

	circle.setRadius(25.0f);
	circle.setPosition(m_body.getPosition());
	circle.setOrigin(m_body.getOrigin());
	circle.setFillColor(sf::Color::Blue);

	scent.m_circle = circle;
	scent.m_timer.start();

	m_scentTrail.push_back(scent);
	m_scentTimer.start();
}

std::vector<Scent> Turtle::getScent()
{
	return m_scentTrail;
}


sf::Sprite Turtle::getSprite()
{
	return m_body;
}

