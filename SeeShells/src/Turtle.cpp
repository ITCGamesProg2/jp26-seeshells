#include "Turtle.h"
#include <iostream>

Turtle::Turtle(AssetManager &t_assetManager, std::vector<sf::Sprite>& t_obstacleSprites)
	: m_assetManager(t_assetManager), 
	m_body(t_assetManager.getTexture("player")),
	m_obstacleSprites(t_obstacleSprites)
{
	m_body.setOrigin({ 16.0f,16.0f }); 


	initAnimation();
}

void Turtle::update(float t_dt)
{
	if (m_scentTrail.size() == 0.0f)
	{
		manageScent();
	}
	if (static_cast<float>(m_scentTimer.getElapsedTime().asSeconds()) >= 1.0f)
	{
		m_scentTimer.reset();
		manageScent();

	}
	
	
	if (checkCollision())
	{
		m_state = CollisionState::COLLIDING;
	}
	else
	{
		m_state = CollisionState::NORMAL;
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


	if (m_state == CollisionState::NORMAL)
	{
		move(t_dt);
		m_currAnimation->update();
	}
	else if (m_state == CollisionState::COLLIDING)
	{
		deflect(t_dt);

		if (m_speed < 0.1)
		{
			move(t_dt);
			m_currAnimation->update();
		}
	}	
}

void Turtle::deflect(float t_dt)
{
	// Calculate impulse based on speed

	float impulseMagnitude = std::abs(m_speed);
	// Normalise the contact vector
	m_contactNormal = m_contactNormal.normalized();
	// Calculate deflection amount
	sf::Vector2f deflectVector =
	{
		impulseMagnitude * m_contactNormal.x * (static_cast<float>(t_dt) / 1000),
		impulseMagnitude * m_contactNormal.y * (static_cast<float>(t_dt) / 1000)
	};
	// Apply impulse along the contact normal
	m_body.move({ deflectVector.x, deflectVector.y });
}

void Turtle::move(float t_dt)
{
	handleMoveInput();
	m_speed = std::clamp(m_speed, -50.0f, 50.0f);
	sf::Vector2f newPos;

	newPos.x = m_body.getPosition().x + std::cos(m_rotation.asRadians()) * m_speed * (t_dt / 1000);
	newPos.y = m_body.getPosition().y + std::sin(m_rotation.asRadians()) * m_speed * (t_dt / 1000);
	m_body.setPosition(newPos);
	m_body.setRotation(m_rotation);
	m_speed *= 0.99;
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


void Turtle::handleHideInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && m_animationState == AnimationState::MOVE)
	{
		m_animationState = AnimationState::SHELL_ON;
		m_animationDelay.restart();
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && m_animationState == AnimationState::HIDE)
	{
		m_animationState = AnimationState::SHELL_OFF;
		m_animationDelay.restart();
	}
}


void Turtle::handleMoveInput()
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

bool Turtle::checkCollision()
{
	for (sf::Sprite const& sprite : m_obstacleSprites)
	{
		if (CollisionDetector::collision(m_body, sprite))
		{
			// Get contact normal vector between tank base and the wall
			m_contactNormal = m_body.getPosition() - sprite.getPosition();
			return true;
		}
	}
	return false;
}

void Turtle::processAnimation()
{
	switch (m_animationState)
	{
	case AnimationState::MOVE:
		break;
	case AnimationState::HIDE:
		break;


	case AnimationState::SHELL_ON:
		if (m_animationDelay.getElapsedTime().asSeconds() > 1.0f)
		{
			m_animationState = AnimationState::HIDE;
			m_animationDelay.stop();
		}
		break;
	case AnimationState::SHELL_OFF:
		if (m_animationDelay.getElapsedTime().asSeconds() > 1.0f)
		{
			m_animationState = AnimationState::MOVE;
			m_animationDelay.stop();
		}
		break;

	}
}

void Turtle::initAnimation()
{
	m_moveAnimation.addFrame(sf::IntRect({ 0,0 }, { 32,32 }));
	m_moveAnimation.addFrame(sf::IntRect({ 32,0 }, { 32,32 }));
	m_moveAnimation.addFrame(sf::IntRect({ 64,0 }, { 32,32 }));
	m_moveAnimation.addFrame(sf::IntRect({ 96,0 }, { 32,32 }));
	m_moveAnimation.addFrame(sf::IntRect({ 128,0 }, { 32,32 }));
	m_moveAnimation.addFrame(sf::IntRect({ 160,0 }, { 32,32 }));


	//Animation m_moveAnimation;
	//Animation m_hideAnimation;
	//Animation m_shellOnAnimation;
	//Animation m_shellOfAnimation;

	m_currAnimation = &m_moveAnimation;
}