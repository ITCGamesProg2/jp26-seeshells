#include "Turtle.h"
#include <iostream>

Turtle::Turtle(AssetManager &t_assetManager)
	: m_assetManager(t_assetManager), 
	m_body(t_assetManager.getTexture("player"))
{
	m_body.setOrigin({ 16.0f,16.0f }); 

	m_moveAnimation.addFrame(sf::IntRect({ 0,0 }, { 32,32 }));
	m_moveAnimation.addFrame(sf::IntRect({ 32,0 }, { 32,32 }));
	m_moveAnimation.addFrame(sf::IntRect({ 64,0 }, { 32,32 }));
	m_moveAnimation.addFrame(sf::IntRect({ 96,0 }, { 32,32 }));
	m_moveAnimation.addFrame(sf::IntRect({ 128,0 }, { 32,32 }));
	m_moveAnimation.addFrame(sf::IntRect({ 160,0 }, { 32,32 }));

	m_currAnimation = &m_moveAnimation;
}

void Turtle::update(float t_dt)
{
	handleKeyInput();
	if (m_scentTrail.size() == 0.0f)
	{
		manageScent();
	}
	if (static_cast<float>(m_scentTimer.getElapsedTime().asSeconds()) >= 1.0f)
	{
		m_scentTimer.reset();
		manageScent();

	}

	for (int i = 0; i < m_scentTrail.size(); i++)
	{
		if (static_cast<float>(m_scentTrail.at(i).m_timer.getElapsedTime().asSeconds()) >= 4.0f)
		{
			Scent temp = m_scentTrail.at(m_scentTrail.size() - 1);
			m_scentTrail.at(i) = temp;
			m_scentTrail.pop_back();
		}
	}
	m_speed = std::clamp(m_speed, -50.0f, 50.0f);
	sf::Vector2f newPos;

	newPos.x = m_body.getPosition().x + std::cos(m_rotation.asRadians()) * m_speed * (t_dt / 1000);
	newPos.y = m_body.getPosition().y + std::sin(m_rotation.asRadians()) * m_speed * (t_dt / 1000);
	m_body.setPosition(newPos);
	m_body.setRotation(m_rotation);
	m_speed *= 0.99;
	m_currAnimation->update();
}

void Turtle::render(sf::RenderWindow& t_window)
{
	for (int i = 0; i < m_scentTrail.size(); i++)
	{
		t_window.draw(m_scentTrail.at(i).m_circle);
	}

	sf::IntRect frame = m_currAnimation->getCurrentFrame();

	m_body.setTextureRect(frame);
	t_window.draw(m_body);
}

void Turtle::manageScent()
{
	Scent scent;
	sf::CircleShape circle;

	circle.setRadius(25.0f);
	circle.setPosition(m_body.getPosition());
	circle.setOrigin(m_body.getOrigin());
	circle.setFillColor(sf::Color::Blue);

	scent.m_circle = circle;
	scent.m_timer.start();

	m_scentTrail.push_back(scent);
	m_scentTimer.start();
}

std::vector<Scent> Turtle::getScent()
{
	return m_scentTrail;
}


sf::Sprite Turtle::getSprite()
{
	return m_body;
}

void Turtle::handleKeyInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		increaseSpeed();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		decreaseSpeed();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		increaseRotation();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		decreaseRotation();
	}
}

void Turtle::increaseSpeed()
{
	m_speed += 2;
}

void Turtle::increaseRotation()
{
	m_rotation += sf::degrees(1.0);

	if (m_rotation.asDegrees() == 360.0)

	{

		m_rotation = sf::degrees(0.0);

	}
}

void Turtle::decreaseSpeed()
{
	m_speed -= 1;
}

void Turtle::decreaseRotation()
{
	m_rotation -= sf::degrees(1.0);

	if (m_rotation.asDegrees() == 360.0)

	{

		m_rotation = sf::degrees(0.0);

	}
}

bool Turtle::isPlayerHiding()
{
	return m_isHiding;
}

