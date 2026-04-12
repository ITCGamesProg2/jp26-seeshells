#include "Environment.h"

Environment::Environment(AssetManager& t_assetManager)
	: m_assetManager(t_assetManager)
{
}

void Environment::render(sf::RenderWindow& t_window)
{
	for (int index = 0; index < MAX_BRANCHES; index++)
	{
		t_window.draw(m_branches[index]);
	}

	for (int index = 0; index < MAX_ROCKS; index++)
	{
		t_window.draw(m_rocks[index]);
	}
}



void Environment::generateObstacles()
{
	sf::Texture const& texture = m_assetManager.getTexture("tiles");

	for (int index = 0; index < MAX_BRANCHES; index++)
	{
		sf::Sprite sprite(texture);
		sprite.setPosition({rand() % 1000 + 200.0f, rand()% 700 + 50.0f});
		m_branches.push_back(sprite);

		int cellID_TL = floor(sprite.getPosition().x / cellWidth) +
			(floor(sprite.getPosition().y / cellHeight) * numCols);
		int cellID_TR = floor((sprite.getPosition().x + sprite.getTexture().getSize().x) / cellWidth) +
			(floor(sprite.getPosition().y / cellHeight) * numCols);

		int cellID_BL = floor(sprite.getPosition().x / cellWidth) +
			(floor((sprite.getPosition().y + sprite.getTexture().getSize().y) / cellHeight) * numCols);
		int cellID_BR = floor((sprite.getPosition().x + sprite.getTexture().getSize().x) / cellWidth) +
			(floor((sprite.getPosition().y + sprite.getTexture().getSize().y) / cellHeight) * numCols);

		// Usage: insert a new map entry
		spatialMap[cellID_TL].push_back(sprite);
		spatialMap[cellID_TR].push_back(sprite);
		spatialMap[cellID_BL].push_back(sprite);
		spatialMap[cellID_BR].push_back(sprite);
	}

	for (int index = 0; index < MAX_ROCKS; index++)
	{
		sf::Sprite sprite(texture);
		sprite.setPosition({ rand() % 1000 + 200.0f, rand() % 700 + 50.0f });
		m_rocks.push_back(sprite);

		int cellID_TL = floor(sprite.getPosition().x / cellWidth) +
			(floor(sprite.getPosition().y / cellHeight) * numCols);
		int cellID_TR = floor((sprite.getPosition().x + sprite.getTexture().getSize().x) / cellWidth) +
			(floor(sprite.getPosition().y / cellHeight) * numCols);

		int cellID_BL = floor(sprite.getPosition().x / cellWidth) +
			(floor((sprite.getPosition().y + sprite.getTexture().getSize().y) / cellHeight) * numCols);
		int cellID_BR = floor((sprite.getPosition().x + sprite.getTexture().getSize().x) / cellWidth) +
			(floor((sprite.getPosition().y + sprite.getTexture().getSize().y) / cellHeight) * numCols);

		// Usage: insert a new map entry
		spatialMap[cellID_TL].push_back(sprite);
		spatialMap[cellID_TR].push_back(sprite);
		spatialMap[cellID_BL].push_back(sprite);
		spatialMap[cellID_BR].push_back(sprite);
	}
}

bool Environment::collision(sf::Sprite t_sprite)
{

	// Calculating cell ID of each corner of plyer
	int cellID_TL = floor(t_sprite.getPosition().x / cellWidth) +
		(floor(t_sprite.getPosition().y / cellHeight) * numCols);


	int cellID_TR = floor((t_sprite.getPosition().x + t_sprite.getTexture().getSize().x) / cellWidth) +
		(floor(t_sprite.getPosition().y / cellHeight) * numCols);

	int cellID_BL = floor(t_sprite.getPosition().x / cellWidth) +
		(floor((t_sprite.getPosition().y + t_sprite.getTexture().getSize().y) / cellHeight) * numCols);


	int cellID_BR = floor((t_sprite.getPosition().x + t_sprite.getTexture().getSize().x) / cellWidth) +
		(floor((t_sprite.getPosition().y + t_sprite.getTexture().getSize().y) / cellHeight) * numCols);


	std::list<sf::Sprite>& obstacles_TL = spatialMap[cellID_TL];
	std::list<sf::Sprite>& obstacles_TR = spatialMap[cellID_TR];
	std::list<sf::Sprite>& obstacles_BL = spatialMap[cellID_BL];
	std::list<sf::Sprite>& obstacles_BR = spatialMap[cellID_BR];

	for (auto& obstacle : obstacles_TL)
	{
		if (obstacle.getGlobalBounds().findIntersection(t_sprite.getGlobalBounds()))
		{
			return true;
		}
	}

	for (auto& obstacle : obstacles_TR)
	{
		if (obstacle.getGlobalBounds().findIntersection(t_sprite.getGlobalBounds()))
		{
			return true;
		}
	}

	for (auto& obstacle : obstacles_BL)
	{
		if (obstacle.getGlobalBounds().findIntersection(t_sprite.getGlobalBounds()))
		{
			return true;
		}
	}

	for (auto& obstacle : obstacles_BR)
	{
		if (obstacle.getGlobalBounds().findIntersection(t_sprite.getGlobalBounds()))
		{
			return true;
		}
	}

	return false;
}

