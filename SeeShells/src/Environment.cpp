#include "Environment.h"

Environment::Environment(AssetManager& t_assetManager)
	: m_assetManager(t_assetManager)
{
}

void Environment::render(sf::RenderWindow& t_window)
{
	for (int index = 0; index < MAX_OBSTACLES; index++)
	{
		t_window.draw(m_obstacles[index]);
	}
}



void Environment::generateObstacles()
{
	sf::Texture const& texture = m_assetManager.getTexture("obstacles");

	for (int index = 0; index < MAX_OBSTACLES; index++)
	{
		sf::Sprite sprite(texture);

		// Randomly decide obstace type
		int type = rand() % 3;
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
		}

		
		// Place the obstacles randomly in the beach (but not in extremities)
		int allowedWidth = ScreenSize::s_width - 260;
		int allowedHeight = ScreenSize::s_height - 100;
		float offsetHeight = 50;
		sprite.setPosition({float(rand() % allowedWidth), rand()% allowedHeight + offsetHeight});
		m_obstacles.push_back(sprite);

		//Update spatial Map with this entry of obstacle 
		int cellID_TL = floor(sprite.getPosition().x / cellWidth) +
			(floor(sprite.getPosition().y / cellHeight) * numCols);
		int cellID_TR = floor((sprite.getPosition().x + sprite.getTexture().getSize().x) / cellWidth) +
			(floor(sprite.getPosition().y / cellHeight) * numCols);

		int cellID_BL = floor(sprite.getPosition().x / cellWidth) +
			(floor((sprite.getPosition().y + sprite.getTexture().getSize().y) / cellHeight) * numCols);
		int cellID_BR = floor((sprite.getPosition().x + sprite.getTexture().getSize().x) / cellWidth) +
			(floor((sprite.getPosition().y + sprite.getTexture().getSize().y) / cellHeight) * numCols);

		// insert a new map entry
		m_spatialMap[cellID_TL].push_back(sprite);
		m_spatialMap[cellID_TR].push_back(sprite);
		m_spatialMap[cellID_BL].push_back(sprite);
		m_spatialMap[cellID_BR].push_back(sprite);
	}
}


bool Environment::entityCollison(sf::Sprite t_entity)
{
	// Values needed to calculate position of all 4 corners of entity
	float posX = t_entity.getPosition().x;
	float posY = t_entity.getPosition().y;
	float length = t_entity.getTextureRect().size.x;
	float height = t_entity.getTextureRect().size.y;


	// figuring out which cell each corner belongs to
	int cellID_TL = floor(posX / cellWidth) + (floor(posY / cellHeight) * numCols);
	int cellID_TR = floor((posX + length) / cellWidth) + (floor(posY / cellHeight) * numCols);
	int cellID_BL = floor(posX / cellWidth) + (floor((posY + height) / cellHeight) * numCols);
	int cellID_BR = floor((posX + length) / cellWidth) + (floor((posY + height) / cellHeight) * numCols);


	// creating references to list of grid wehre entity's corners are located so 
	// that we can check them to see if any obstacle located there
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

std::vector<sf::Sprite>& Environment::getObstacles()
{
	return m_obstacles;
}