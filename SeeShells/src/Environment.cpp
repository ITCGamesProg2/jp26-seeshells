#include "Environment.h"

Environment::Environment(AssetManager& t_assetManager)
	: m_assetManager(t_assetManager)
{
}

void Environment::render(sf::RenderWindow& t_window)
{
	for (int index = 0; index < MAX_OBSTACLES; index++)
	{
		t_window.draw(m_branches[index]);
	}
}



void Environment::generateObstacles()
{
	sf::Texture const& texture = m_assetManager.getTexture("obstacles");

	for (int index = 0; index < MAX_OBSTACLES; index++)
	{
		sf::Sprite sprite(texture);

		// Randomly decide obstace type
		int type = rand() % 4;
		switch (type)
		{
		case 0:
			sprite.setTextureRect(sf::IntRect({0,0}, {32,32}));
			break;
		case 1:
			sprite.setTextureRect(sf::IntRect({ 32,0 }, { 32,32 }));
			break;
		case 2:
			sprite.setTextureRect(sf::IntRect({ 0,32 }, { 32,32 }));
			break;
		case 3:
			sprite.setTextureRect(sf::IntRect({ 32,32 }, { 32,32 }));
			break;
		}

		
		sprite.setPosition({rand() % 1200 + 100.0f, rand()% 700 + 50.0f});
		m_branches.push_back(sprite);

		//Update spatial Map with this entry of obstacle 
		int cellID_TL = floor(sprite.getPosition().x / cellWidth) +
			(floor(sprite.getPosition().y / cellHeight) * numCols);
		int cellID_TR = floor((sprite.getPosition().x + sprite.getTexture().getSize().x) / cellWidth) +
			(floor(sprite.getPosition().y / cellHeight) * numCols);

		int cellID_BL = floor(sprite.getPosition().x / cellWidth) +
			(floor((sprite.getPosition().y + sprite.getTexture().getSize().y) / cellHeight) * numCols);
		int cellID_BR = floor((sprite.getPosition().x + sprite.getTexture().getSize().x) / cellWidth) +
			(floor((sprite.getPosition().y + sprite.getTexture().getSize().y) / cellHeight) * numCols);

		// Usage: insert a new map entry
		m_spatialMap[cellID_TL].push_back(sprite);
		m_spatialMap[cellID_TR].push_back(sprite);
		m_spatialMap[cellID_BL].push_back(sprite);
		m_spatialMap[cellID_BR].push_back(sprite);
	}
}


bool Environment::entityCollison(sf::Sprite t_entity)
{
	int cellID_TL = floor(t_entity.getPosition().x / cellWidth) +
		(floor(t_entity.getPosition().y / cellHeight) * numCols);
	int cellID_TR = floor((t_entity.getPosition().x + t_entity.getTexture().getSize().x) / cellWidth) +
		(floor(t_entity.getPosition().y / cellHeight) * numCols);
	int cellID_BL = floor(t_entity.getPosition().x / cellWidth) +
		(floor((t_entity.getPosition().y + t_entity.getTexture().getSize().y) / cellHeight) * numCols);
	int cellID_BR = floor((t_entity.getPosition().x + t_entity.getTexture().getSize().x) / cellWidth) +
		(floor((t_entity.getPosition().y + t_entity.getTexture().getSize().y) / cellHeight) * numCols);

	std::list<sf::Sprite>& entity_TL = m_spatialMap[cellID_TL];
	std::list<sf::Sprite>& entity_TR = m_spatialMap[cellID_TR];
	std::list<sf::Sprite>& entity_BL = m_spatialMap[cellID_BL];
	std::list<sf::Sprite>& entity_BR = m_spatialMap[cellID_BR];


	for (auto& obstacle : entity_TL)
	{
		if (obstacle.getGlobalBounds().findIntersection(t_entity.getGlobalBounds()))
		{
			return true;
		}
	}

	for (auto& obstacle : entity_TR)
	{
		if (obstacle.getGlobalBounds().findIntersection(t_entity.getGlobalBounds()))
		{
			return true;
		}
	}

	for (auto& obstacle : entity_BL)
	{
		if (obstacle.getGlobalBounds().findIntersection(t_entity.getGlobalBounds()))
		{
			return true;
		}
	}

	for (auto& obstacle : entity_BR)
	{
		if (obstacle.getGlobalBounds().findIntersection(t_entity.getGlobalBounds()))
		{
			return true;
		}
	}

	return false;
}