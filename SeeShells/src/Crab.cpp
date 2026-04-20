#include "Crab.h"

Crab::Crab(AssetManager& t_assetManager, std::map<int, std::list<sf::Sprite>>& t_spatialMap)
	: m_assetManager(t_assetManager),
	m_body(t_assetManager.getTexture("crab")), m_moveDelay(5)
{
	m_body.setPosition({ 100,100 });
	updateSpatialMap(t_spatialMap);
}

void Crab::update(float t_dt)
{
	move(t_dt);
}

void Crab::updateSpatialMap(std::map<int, std::list<sf::Sprite>>& t_spatialMap)
{
	float posX = m_body.getPosition().x;
	float posY = m_body.getPosition().y;
	float length = m_body.getTextureRect().size.x;
	float height = m_body.getTextureRect().size.y;


	int cellID_TL = floor(posX / cellWidth) + (floor(posY / cellHeight) * numCols);
	int cellID_TR = floor((posX + length) / cellWidth) + (floor(posY / cellHeight) * numCols);
	int cellID_BL = floor(posX / cellWidth) + (floor((posY + height) / cellHeight) * numCols);
	int cellID_BR = floor((posX + length) / cellWidth) + (floor((posY + height) / cellHeight) * numCols);

	// Usage: insert a new map entry
	t_spatialMap[cellID_TL].push_back(m_body);
	t_spatialMap[cellID_TR].push_back(m_body);
	t_spatialMap[cellID_BL].push_back(m_body);
	t_spatialMap[cellID_BR].push_back(m_body);
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
