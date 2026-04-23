#include "Game.h"
#include <iostream>

// Our target FPS
static float const FPS{ 60.0f };

////////////////////////////////////////////////////////////
Game::Game(AssetManager& t_assetManager)
	: m_window(sf::VideoMode({ ScreenSize::s_width, ScreenSize::s_height }, 32), "SFML Playground", sf::Style::Default),
	m_assetManager(t_assetManager),
	m_turtle(m_assetManager, m_environment.getObstacles()), m_environment(m_assetManager),m_background(m_backgroundT)
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
	m_background.setPosition({ 0.0f,0.0f });

	m_environment.generateObstacles();

	m_turtle.addObserver(&m_audioSystem);
	m_music.setLooping(true);

	spawnEnemies();

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
	switch (m_state)
	{
	case GameState::START:
		// If player types PLAY starts game
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
		{
			lastPressed = sf::Keyboard::Key::P;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L))
		{
			if (lastPressed == sf::Keyboard::Key::P)
			{
				lastPressed = sf::Keyboard::Key::L;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			if (lastPressed == sf::Keyboard::Key::L)
			{
				lastPressed = sf::Keyboard::Key::A;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y))
		{
			if (lastPressed == sf::Keyboard::Key::A)
			{
				m_state = GameState::PLAY;
				m_music.play();
			}
		}
		break;

	case GameState::PLAY:
		// Timer decreasing
		if (m_timer >= 0.0f)
		{
			m_timer -= dt / 1000;
		}
		else
		{
			m_state = GameState::LOSE;
		}

		//Checks for win
		if (m_turtle.getWin())
		{
			m_state = GameState::WIN;
			m_turtle.notifyAll(Event::WIN);
			m_music.stop();
		}

		m_turtle.update(dt);

		for (int i = 0; i < m_crabs.size(); i++)
		{
			m_crabs.at(i).update(dt);
		}
		for (int i = 0; i < m_birds.size(); i++)
		{
			m_birds.at(i).update(dt, m_turtle.getScent(), m_turtle.getSprite(), m_turtle.isPlayerHiding());

			if (m_birds.at(i).isPlayerDead())
			{
				m_state = GameState::LOSE;
				m_turtle.notifyAll(Event::DIE);
				m_music.stop();
			}
		}

		updateSpatialMap();
		checkCollision();
		break;

	case GameState::WIN:
	case GameState::LOSE:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
		{
			lastPressed = sf::Keyboard::Key::P;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L))
		{
			if (lastPressed == sf::Keyboard::Key::P)
			{
				lastPressed = sf::Keyboard::Key::L;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			if (lastPressed == sf::Keyboard::Key::L)
			{
				lastPressed = sf::Keyboard::Key::A;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y))
		{
			if (lastPressed == sf::Keyboard::Key::A)
			{
				reset();
			}
		}
		break;

	default:
		break;
	}
}


void Game::updateSpatialMap()
{
	m_spatialMap.clear();

	for (int i = 0; i < m_crabs.size(); i++)
	{
		m_crabs.at(i).updateSpatialMap(m_spatialMap);
	}
}



////////////////////////////////////////////////////////////
void Game::checkCollision()
{
	if (entityCollision())
	{
		m_state = GameState::LOSE;
		m_turtle.notifyAll(Event::DIE);
	}

	for (int i = 0; i < m_crabs.size(); i++)
	{
		sf::Sprite crabby = m_crabs.at(i).getSprite();
		if (m_environment.entityCollison(crabby))
		{
			m_crabs.at(i).changeDirection();
		}
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
	sf::Text text1(m_arialFont);
	sf::Text text2(m_arialFont);
	switch (m_state)
	{
	case GameState::START:
		m_window.clear(sf::Color(173, 216, 230, 255));
		text1.setString("See Shells");
		text1.setCharacterSize(200);
		text1.setFillColor(sf::Color(117, 184, 79));
		text1.setPosition({ 360.0f,180.0f });

		text2.setString("Type PLAY to play");
		text2.setCharacterSize(50);
		text2.setFillColor(sf::Color(117, 184, 79));
		text2.setPosition({ 525.0f,550.0f });

		m_window.draw(text1);
		m_window.draw(text2);
		break;
	case GameState::PLAY:
		text1.setString(std::to_string(static_cast<int>(m_timer)));
		text1.setCharacterSize(100);
		text1.setFillColor(sf::Color(117, 184, 79));
		text1.setPosition({ 1350.0f,10.0f });
		m_window.draw(m_background);
		m_environment.render(m_window);
		m_turtle.render(m_window);
		for (int i = 0; i < m_crabs.size(); i++)
		{
			m_crabs.at(i).render(m_window);
		}
		for (int i = 0; i < m_birds.size(); i++)
		{
			m_birds.at(i).render(m_window);
		}

		
		m_window.draw(text1);
		break;
	case GameState::WIN:
	case GameState::LOSE:
		m_window.clear(sf::Color(173, 216, 230, 255));
		if (m_state == GameState::LOSE)
		{
			text1.setString("Gameover");
		}
		else
		{
			text1.setString("You won");
		}
		text1.setCharacterSize(200);
		text1.setFillColor(sf::Color(117, 184, 79));
		text1.setPosition({ 390.0f,180.0f });

		text2.setString("Type PLAY to play again");
		text2.setCharacterSize(50);
		text2.setFillColor(sf::Color(117, 184, 79));
		text2.setPosition({ 495.0f,550.0f });

		m_window.draw(text1);
		m_window.draw(text2);
		break;
	default:
		break;
	}

#ifdef TEST_FPS
	m_window.draw(x_updateFPS);
	m_window.draw(x_drawFPS);
#endif
	m_window.display();
}

void Game::reset()
{
	m_timer = 60.0f;
	m_birds.clear();
	m_crabs.clear();

	spawnEnemies();

	m_turtle.reset();

	m_state = GameState::PLAY;
	m_music.stop();
	m_music.play();

}

void Game::spawnEnemies()
{
	m_birds.reserve(NUM_OF_BIRDS);
	m_crabs.reserve(NUM_OF_CRABS);
	for (int i = 0; i < NUM_OF_BIRDS; i++)
	{
		float posx = rand() % 1050 + 150;
		float posy = rand() % 700 + 100;
		m_birds.emplace_back(m_assetManager, sf::Vector2f{ posx,posy });
	}
	for (int i = 0; i < NUM_OF_CRABS; i++)
	{
		float posx = rand() % 1050 + 150;
		float posy = rand() % 700 + 100;
		m_crabs.emplace_back(m_assetManager, m_spatialMap, sf::Vector2f{ posx,posy });
	}
}














