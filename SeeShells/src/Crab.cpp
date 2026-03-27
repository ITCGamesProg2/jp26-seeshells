#include "Crab.h"

Crab::Crab(AssetManager& t_assetManager)
	: m_assetManager(t_assetManager),
	m_body(t_assetManager.getTexture("crab")), m_moveDelay(5)
{
	m_body.setPosition({ 100,100 });
}

void Crab::update(float t_dt)
{
	move(t_dt);
}

void Crab::move(float t_dt)
{
	if (m_moveTimer.getElapsedTime().asSeconds() > m_moveDelay)
	{
		m_moveDelay = 5;
		m_moveTimer.restart();
		m_goingLeft = !m_goingLeft;
	}


	sf::Vector2f pos = m_body.getPosition();
	if (m_goingLeft)
	{
		pos.x -= m_speed * (t_dt / 1000);
	}
	else
	{
		pos.x += m_speed * (t_dt / 1000);
	}

	m_body.setPosition(pos);
}

void Crab::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_body);
}

