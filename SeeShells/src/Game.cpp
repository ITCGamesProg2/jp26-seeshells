#include "Game.h"
#include <iostream>

// Our target FPS
static float const FPS{ 60.0f };

////////////////////////////////////////////////////////////
Game::Game(AssetManager& t_assetManager)
	: m_window(sf::VideoMode({ ScreenSize::s_width, ScreenSize::s_height }, 32), "SFML Playground", sf::Style::Default), 
	m_bird(t_assetManager, {400.0f, 400.0f}),
	m_turtle(t_assetManager), m_crab(t_assetManager, m_spatialMap), m_environment(t_assetManager)
{
	init();
}

////////////////////////////////////////////////////////////
void Game::init()
{
	// Really only necessary is our target FPS is greater than 60.
	m_window.setVerticalSyncEnabled(true);

	if (!m_arialFont.openFromFile("BebasNeue.otf"))
	{
		std::cerr << "Error loading font file";
	}

	m_environment.generateObstacles();

	m_turtle.addObserver(&m_audioSystem);

#ifdef TEST_FPS
	x_updateFPS.setFont(m_arialFont);
	x_updateFPS.setPosition(sf::Vector2f{ 20.0f, 300.0f });
	x_updateFPS.setCharacterSize(24);
	x_updateFPS.setFillColor(sf::Color::White);
	x_drawFPS.setFont(m_arialFont);
	x_drawFPS.setPosition(sf::Vector2f{ 20.0f, 350.0f });
	x_drawFPS.setCharacterSize(24);
	x_drawFPS.setFillColor(sf::Color::White);
#endif
}

////////////////////////////////////////////////////////////
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	
	sf::Time timePerFrame = sf::seconds(1.0f / FPS); // 60 fps

	


	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame.asMilliseconds()); //60 fps
#ifdef TEST_FPS
			x_secondTime += timePerFrame;
			x_updateFrameCount++;
			if (x_secondTime.asSeconds() > 1)
			{
				std::string updatesPS = "UPS " + std::to_string(x_updateFrameCount - 1);
				x_updateFPS.setString(updatesPS);
				std::string drawsPS = "DPS " + std::to_string(x_drawFrameCount);
				x_drawFPS.setString(drawsPS);
				x_updateFrameCount = 0;
				x_drawFrameCount = 0;
				x_secondTime = sf::Time::Zero;
			}
#endif
		}
		render(); // as many as possible
#ifdef TEST_FPS
		x_drawFrameCount++;
#endif
	}
}

////////////////////////////////////////////////////////////
void Game::processEvents()
{

	// get the next event from the event queue (if there is one)
	while (const std::optional event = m_window.pollEvent())
	{
		// "close requested" event: we close the window
		if (event->is<sf::Event::Closed>())
		{
			m_window.close();
		}
		if (event->is<sf::Event::KeyPressed>())
		{
			processKeyPressed(event);
		}		
	}
}

////////////////////////////////////////////////////////////
void Game::processKeyPressed(const std::optional<sf::Event>& t_event)
{	
	
	sf::Event::KeyPressed const* keyPressed = t_event->getIf<sf::Event::KeyPressed>();
	if (keyPressed != nullptr)
	{
		switch (keyPressed->code)
		{
			case sf::Keyboard::Key::Escape:
				m_window.close();
				break;
			case sf::Keyboard::Key::Up:
				// Up key was pressed...
				break;
			default:
				break;
		}
	}
}

////////////////////////////////////////////////////////////
void Game::update(double dt)
{
	m_turtle.update(dt);

	m_crab.update(dt);
	m_bird.update(dt, m_turtle.getScent());

	updateSpatialMap();
	checkCollision();

}


void Game::updateSpatialMap()
{
	m_spatialMap.clear();

	m_crab.updateSpatialMap(m_spatialMap);
}



////////////////////////////////////////////////////////////
void Game::checkCollision()
{
	sf::Sprite playa = m_turtle.getSprite();
	if (m_environment.entityCollison(playa))
	{
		std::cout << "Player collided with tile\n";
		
	}

	if (entityCollision())
	{
		m_turtle.notifyAll(Event::DIE);
	}

	sf::Sprite crabby = m_crab.getSprite();
	if (m_environment.entityCollison(crabby))
	{
		std::cout << "crabby collided with tile\n";
	}
}

bool Game::entityCollision()
{
	sf::Sprite playa = m_turtle.getSprite();

	float posX = playa.getPosition().x;
	float posY = playa.getPosition().y;
	float length = playa.getTextureRect().size.x;
	float height = playa.getTextureRect().size.y;


	int cellID_TL = floor(posX / cellWidth) + (floor(posY / cellHeight) * numCols);
	int cellID_TR = floor((posX + length) / cellWidth) + (floor(posY / cellHeight) * numCols);
	int cellID_BL = floor(posX / cellWidth) + (floor((posY + height) / cellHeight) * numCols);
	int cellID_BR = floor((posX + length) / cellWidth) + (floor((posY + height) / cellHeight) * numCols);

	std::list<sf::Sprite>& entity_TL = m_spatialMap[cellID_TL];
	std::list<sf::Sprite>& entity_TR = m_spatialMap[cellID_TR];
	std::list<sf::Sprite>& entity_BL = m_spatialMap[cellID_BL];
	std::list<sf::Sprite>& entity_BR = m_spatialMap[cellID_BR];


	for (auto& obstacle : entity_TL)
	{
		if (obstacle.getGlobalBounds().findIntersection(playa.getGlobalBounds()))
		{
			return true;
		}
	}

	for (auto& obstacle : entity_TR)
	{
		if (obstacle.getGlobalBounds().findIntersection(playa.getGlobalBounds()))
		{
			return true;
		}
	}

	for (auto& obstacle : entity_BL)
	{
		if (obstacle.getGlobalBounds().findIntersection(playa.getGlobalBounds()))
		{
			return true;
		}
	}

	for (auto& obstacle : entity_BR)
	{
		if (obstacle.getGlobalBounds().findIntersection(playa.getGlobalBounds()))
		{
			return true;
		}
	}

	return false;
}

////////////////////////////////////////////////////////////
void Game::render()
{
	m_window.clear(sf::Color(0, 0, 0, 0));

	m_turtle.render(m_window);
	m_crab.render(m_window);
	m_bird.render(m_window);

	m_environment.render(m_window);

#ifdef TEST_FPS
	m_window.draw(x_updateFPS);
	m_window.draw(x_drawFPS);
#endif
	m_window.display();
}














