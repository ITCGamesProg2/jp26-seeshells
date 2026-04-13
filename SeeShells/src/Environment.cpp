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



void Environment::generateObstacles(std::map<int, std::list<sf::Sprite>>& t_spatialMap)
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

		
		sprite.setPosition({rand() % 1000 + 200.0f, rand()% 700 + 50.0f});
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
		t_spatialMap[cellID_TL].push_back(sprite);
		t_spatialMap[cellID_TR].push_back(sprite);
		t_spatialMap[cellID_BL].push_back(sprite);
		t_spatialMap[cellID_BR].push_back(sprite);
	}
}


