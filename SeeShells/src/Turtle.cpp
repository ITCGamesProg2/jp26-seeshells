#include "Turtle.h"

Turtle::Turtle(AssetManager &t_assetManager, std::vector<sf::Sprite>& t_obstacleSprites)
	: m_assetManager(t_assetManager), 
	m_body(t_assetManager.getTexture("player")),
	m_obstacleSprites(t_obstacleSprites)
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
	m_direction = { 0,0 };


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		m_direction.y -= 1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		m_direction.x -= 1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		m_direction.y += 1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		m_direction.x += 1;
	}

	if (m_direction.x != 0 && m_direction.y != 0)
	{
		m_direction = m_direction.normalized();
	}
	else if (m_direction.x != 0 || m_direction.y != 0)
	{
		notifyAll(Event::MOVE);
		rotate();
	}
	

	

	sf::Vector2f pos = m_body.getPosition();
	pos.x += m_direction.x * m_speed * (t_dt/1000);
	pos.y += m_direction.y * m_speed * (t_dt/1000);

	m_body.setPosition(pos);
}

void Turtle::rotate()
{
	float currentHeading = m_body.getRotation().asDegrees();
	sf::Vector2 pos = m_body.getPosition();


	pos += m_direction;
	float distance = sqrt((pos.x * pos.x) + (pos.y * pos.y));

	m_body.setRotation(m_direction.angle());



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
