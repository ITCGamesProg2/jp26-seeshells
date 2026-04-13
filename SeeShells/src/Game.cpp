#include "Game.h"
#include <iostream>

// Our target FPS
static float const FPS{ 60.0f };

////////////////////////////////////////////////////////////
Game::Game(AssetManager& t_assetManager)
	: m_window(sf::VideoMode({ ScreenSize::s_width, ScreenSize::s_height }, 32), "SFML Playground", sf::Style::Default), 
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

	m_environment.generateObstacles(m_spatialMap);

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
	m_crab.update(dt, m_spatialMap);


	checkCollision();

}



////////////////////////////////////////////////////////////
bool Game::checkCollision()
{
	sf::Sprite playa = m_turtle.getSprite();

	// Calculating cell ID of each corner of plyer (?or opponent if need be?)
	int playaID_TL = floor(playa.getPosition().x / cellWidth) +
		(floor(playa.getPosition().y / cellHeight) * numCols);
	int playaID_TR = floor((playa.getPosition().x + playa.getTexture().getSize().x) / cellWidth) +
		(floor(playa.getPosition().y / cellHeight) * numCols);
	int playaID_BL = floor(playa.getPosition().x / cellWidth) +
		(floor((playa.getPosition().y + playa.getTexture().getSize().y) / cellHeight) * numCols);
	int playaID_BR = floor((playa.getPosition().x + playa.getTexture().getSize().x) / cellWidth) +
		(floor((playa.getPosition().y + playa.getTexture().getSize().y) / cellHeight) * numCols);

	std::list<sf::Sprite>& playa_TL = m_spatialMap[playaID_TL];
	std::list<sf::Sprite>& playa_TR = m_spatialMap[playaID_TR];
	std::list<sf::Sprite>& playa_BL = m_spatialMap[playaID_BL];
	std::list<sf::Sprite>& playa_BR = m_spatialMap[playaID_BR];


	if (environmentCollision(playa, playa_TL, playa_BL, playa_TR, playa_TL))
	{
		std::cout << "Player collided with tile\n";
	}

	sf::Sprite crabby = m_crab.getSprite();

	// Calculating cell ID of each corner of plyer (?or opponent if need be?)
	int crabbyID_TL = floor(crabby.getPosition().x / cellWidth) +
		(floor(crabby.getPosition().y / cellHeight) * numCols);
	int crabbyID_TR = floor((crabby.getPosition().x + crabby.getTexture().getSize().x) / cellWidth) +
		(floor(crabby.getPosition().y / cellHeight) * numCols);
	int crabbyID_BL = floor(crabby.getPosition().x / cellWidth) +
		(floor((crabby.getPosition().y + crabby.getTexture().getSize().y) / cellHeight) * numCols);
	int crabbyID_BR = floor((crabby.getPosition().x + crabby.getTexture().getSize().x) / cellWidth) +
		(floor((crabby.getPosition().y + crabby.getTexture().getSize().y) / cellHeight) * numCols);

	std::list<sf::Sprite>& crabby_TL = m_spatialMap[crabbyID_TL];
	std::list<sf::Sprite>& crabby_TR = m_spatialMap[crabbyID_TR];
	std::list<sf::Sprite>& crabby_BL = m_spatialMap[crabbyID_BL];
	std::list<sf::Sprite>& crabby_BR = m_spatialMap[crabbyID_BR];

	if (environmentCollision(crabby, crabby_TL, crabby_TR, crabby_BL, crabby_BR))
	{
		std::cout << "Crab collided with environment\n";
	}


	if (playerCrabCollision(playa, playa_TL, playa_BL, playa_TR, playa_TL))

	return false;
}

////////////////////////////////////////////////////////////
bool Game::environmentCollision(sf::Sprite t_entity, std::list<sf::Sprite>& t_obstacles_TL, 
								std::list<sf::Sprite>& t_obstacles_BL,
								std::list<sf::Sprite>& t_obstacles_TR,
								std::list<sf::Sprite>& t_obstacles_BR)
{
	for (auto& obstacle : t_obstacles_TL)
	{
		if (obstacle.getGlobalBounds().findIntersection(t_entity.getGlobalBounds()))
		{
			return true;
		}
	}

	for (auto& obstacle : t_obstacles_TR)
	{
		if (obstacle.getGlobalBounds().findIntersection(t_entity.getGlobalBounds()))
		{
			return true;
		}
	}

	for (auto& obstacle : t_obstacles_BL)
	{
		if (obstacle.getGlobalBounds().findIntersection(t_entity.getGlobalBounds()))
		{
			return true;
		}
	}

	for (auto& obstacle : t_obstacles_BR)
	{
		if (obstacle.getGlobalBounds().findIntersection(t_entity.getGlobalBounds()))
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

	m_environment.render(m_window);

#ifdef TEST_FPS
	m_window.draw(x_updateFPS);
	m_window.draw(x_drawFPS);
#endif
	m_window.display();
}














