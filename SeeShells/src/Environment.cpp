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
	}

	for (int index = 0; index < MAX_ROCKS; index++)
	{
		sf::Sprite sprite(texture);
		sprite.setPosition({ rand() % 1000 + 200.0f, rand() % 700 + 50.0f });
		m_rocks.push_back(sprite);
	}
}

