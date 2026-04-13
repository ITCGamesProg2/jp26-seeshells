#include "Crab.h"

Crab::Crab(AssetManager& t_assetManager, std::map<int, std::list<sf::Sprite>>& t_spatialMap)
	: m_assetManager(t_assetManager),
	m_body(t_assetManager.getTexture("crab")), m_moveDelay(5)
{
	m_body.setPosition({ 100,100 });
	//updateSpatialMap(t_spatialMap);
}

void Crab::updateSpatialMap(std::map<int, std::list<sf::Sprite>>& t_spatialMap)
{
	//Update spatial Map with this entry of obstacle 
	int cellID_TL = floor(m_body.getPosition().x / cellWidth) +
		(floor(m_body.getPosition().y / cellHeight) * numCols);
	int cellID_TR = floor((m_body.getPosition().x + m_body.getTexture().getSize().x) / cellWidth) +
		(floor(m_body.getPosition().y / cellHeight) * numCols);

	int cellID_BL = floor(m_body.getPosition().x / cellWidth) +
		(floor((m_body.getPosition().y + m_body.getTexture().getSize().y) / cellHeight) * numCols);
	int cellID_BR = floor((m_body.getPosition().x + m_body.getTexture().getSize().x) / cellWidth) +
		(floor((m_body.getPosition().y + m_body.getTexture().getSize().y) / cellHeight) * numCols);

	// Usage: insert a new map entry
	t_spatialMap[cellID_TL].push_back(m_body);
	t_spatialMap[cellID_TR].push_back(m_body);
	t_spatialMap[cellID_BL].push_back(m_body);
	t_spatialMap[cellID_BR].push_back(m_body);
}

void Crab::update(float t_dt, std::map<int, std::list<sf::Sprite>>& t_spatialMap)
{
	move(t_dt);
	//updateSpatialMap(t_spatialMap);
}

bool Crab::collision(sf::Sprite t_sprite, std::map<int, std::list<sf::Sprite>>& t_spatialMap)
{
	return false;
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

sf::Sprite Crab::getSprite()
{
	return m_body;
}

