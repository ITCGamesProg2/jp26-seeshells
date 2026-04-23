#include "Bird.h"

Bird::Bird(AssetManager& t_assetManager, sf::Vector2f t_pos):
	m_assetManager(t_assetManager),
	m_body(t_assetManager.getTexture("bird")),
	m_center(t_pos),
	m_visionCone(sf::PrimitiveType::TriangleFan, 12)
{
	m_body.setPosition({ 600.0f,600.0f });
	m_body.setOrigin({ 16.0f,16.0f });
	m_angle = 0.0f;
	//m_diffUpdating = 10000;
	m_moveAnimation.addFrame(sf::IntRect({ 0,0 }, { 32,32 }));
	m_moveAnimation.addFrame(sf::IntRect({ 32,0 }, { 32,32 }));
	m_moveAnimation.addFrame(sf::IntRect({ 64,0 }, { 32,32 }));
	m_moveAnimation.addFrame(sf::IntRect({ 96,0 }, { 32,32 }));
	m_moveAnimation.addFrame(sf::IntRect({ 128,0 }, { 32,32 }));
	m_moveAnimation.addFrame(sf::IntRect({ 160,0 }, { 32,32 }));

	m_currAnimation = &m_moveAnimation;
	m_body.setScale({ 2,2 });

	m_killTimer.reset();
	m_playerDead = false;
}
void Bird::update(float dt, std::vector<Scent> &t_playerScent, sf::Sprite& t_turtle, bool t_isHiding)
{
	if (m_ignoreScentTimer > 0.0f)
	{
		m_ignoreScentTimer -= dt / 1000.0f;
	}
	if (m_alertTimer > 0.0f)
	{
		m_alertTimer -= dt / 1000.0f;
	}
	switch (m_state)
	{
	case BirdState::SEARCHING:
	case BirdState::ALERT:
		if (m_alertTimer <= 0.0f)
		{
			m_state = BirdState::SEARCHING;
		}
		if(m_ignoreScentTimer <= 0.0f)
		{
			collisionVisionConeScent(t_playerScent);
		}
		collisionVisionConeScent(t_playerScent);
		break;
	case BirdState::PURSUING:
	case BirdState::ATTACKING:
		if (!t_isHiding)
		{
			if (!m_killTimer.isRunning())
			{
				collisionVisionConeTurtle(t_turtle);
			}
			else
			{
				if (static_cast<float>(m_killTimer.getElapsedTime().asSeconds()) >= 1.5f)
				{
					if (collisionVisionConeTurtle(t_turtle))
					{
						m_playerDead = true;
						m_killTimer.reset();
					}
					else
					{
						m_killTimer.reset();
						m_state = BirdState::ALERT;
						m_alertTimer = 3.0f;
					}
				}
			}
		}
		break;
	default:
		break;
	}

	move(dt);
	visionCone();

	m_currAnimation->update();
}

void Bird::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_visionCone);
	sf::IntRect frame = m_currAnimation->getCurrentFrame();

	m_body.setTextureRect(frame);
	t_window.draw(m_body);
}

void Bird::visionCone()
{
	sf::Vector2f spritePos = m_body.getPosition();
	sf::Color coneColor;
	float addingToRotation = 0.0f;
	
	switch (m_state)
	{
	case BirdState::SEARCHING:
		m_angleOfFan = 45.0f;
		m_reachOfFan = 100.0f;
		addingToRotation = 90.0f;
		coneColor = sf::Color(0, 255, 0, 50);
		break;
	case BirdState::ALERT:
		addingToRotation = 90.0f;
		m_angleOfFan = 35.0f;
		m_reachOfFan = 100.0f;
		coneColor = sf::Color(255, 255, 0, 50);
		break;
	case BirdState::PURSUING:
		addingToRotation = -270.0f;
		m_angleOfFan = 25.0f;
		m_reachOfFan = 125.0f;
		coneColor = sf::Color(255, 165, 0, 50);
		break;
	case BirdState::ATTACKING:
		addingToRotation = -270.0f;
		m_angleOfFan = 15.0f;
		m_reachOfFan = 150.0f;
		coneColor = sf::Color(255, 0, 0, 50);
		break;
	default:
		break;
	}

	m_visionCone[0].position = { spritePos.x,spritePos.y };
	m_visionCone[0].color = coneColor;

	for (int i = 1; i < 12; i++)
	{
		float relAngle = (i - 1) * (m_angleOfFan / 10) - (m_angleOfFan / 2.0f);

		float totalAngle = m_body.getRotation().asDegrees() + relAngle + addingToRotation;

		float radAngle = totalAngle * (3.14159f / 180.f);

		m_visionCone[i].position = { spritePos.x + std::cos(radAngle) * m_reachOfFan, spritePos.y + std::sin(radAngle) * m_reachOfFan };
		m_visionCone[i].color = coneColor;
	}
}

void Bird::move(float dt)
{
	sf::Vector2f newPos;

	switch (m_state)
	{
	case BirdState::SEARCHING:
		m_angle += (m_speed * dt / 1000);

		newPos.x = m_center.x + m_radius * std::cos(m_angle);
		newPos.y = m_center.y + m_radius * std::sin(m_angle);
  
		m_body.setRotation(sf::radians(m_angle));
		m_body.setPosition(newPos);
		break;
	case BirdState::ALERT:
		m_angle += ((m_speed/2) * dt / 1000);

		newPos.x = m_center.x + m_radius * std::cos(m_angle);
		newPos.y = m_center.y + m_radius * std::sin(m_angle);

		m_body.setRotation(sf::radians(m_angle));
		m_body.setPosition(newPos);
		break;
	case BirdState::PURSUING:
		
		m_body.setRotation(lookAt(m_chasingPoint));
		if (m_diffUpdating > 10.0f)
		{
			newPos.x = m_body.getPosition().x + (m_direction.x * m_speed * dt / 10);
			newPos.y = m_body.getPosition().y + (m_direction.y * m_speed * dt / 10);

			m_body.setPosition(newPos);
		}
		else
		{
			
			m_body.setPosition(m_chasingPoint);
			if (m_returning)
			{
				m_returning = false;
				m_state = BirdState::ALERT;
				m_alertTimer = 3.0f;
				m_ignoreScentTimer = 2.0f;
			}
			if (m_chasing)
			{
				m_chasingPoint = m_movingFrom;
				m_chasing = false;
				m_returning = true;
			}
		}

		break;
	case BirdState::ATTACKING:
		m_body.setRotation(lookAt(m_chasingPoint));
		if (m_diffUpdating > 10.0f)
		{
			newPos.x = m_body.getPosition().x + (m_direction.x * m_speed * dt / 10);
			newPos.y = m_body.getPosition().y + (m_direction.y * m_speed * dt / 10);

			m_body.setPosition(newPos);
		}
		else
		{

			m_body.setPosition(m_chasingPoint);
			if (m_returning)
			{
				m_returning = false;
				m_state = BirdState::ALERT;
				m_alertTimer = 3.0f;
				m_ignoreScentTimer = 2.0f;
			}
			if (m_chasing)
			{
				m_chasingPoint = m_movingFrom;
				m_chasing = false;
				m_returning = true;
			}
		}
		break;
	default:
		break;
	}

	if (m_angle >= 360.0f)
	{
		m_angle = 0.0f;
	}
}

void Bird::collisionVisionConeScent(std::vector<Scent> &t_playerScent)
{
	sf::Vector2f birdPos = m_body.getPosition();
	for(int i = t_playerScent.size() - 1; i >= 0; i--)
	{
		if (t_playerScent.at(i).m_circle.getGlobalBounds().findIntersection(m_visionCone.getBounds()))
		{
			m_state = BirdState::PURSUING;
			m_chasingPoint = t_playerScent.at(i).m_circle.getPosition();
			m_movingFrom = m_body.getPosition();
			m_chasing = true;
			m_returning = false;
			return;
		}
	}
}

sf::Angle Bird::lookAt(sf::Vector2f t_pointToLookAt)
{
	sf::Angle angle;
	m_direction = t_pointToLookAt - m_body.getPosition();
	m_diffUpdating = std::sqrt((t_pointToLookAt.x - m_body.getPosition().x) * (t_pointToLookAt.x - m_body.getPosition().x) + (t_pointToLookAt.y - m_body.getPosition().y) * (t_pointToLookAt.y - m_body.getPosition().y));
	if (m_direction != sf::Vector2f{ 0.0f,0.0f })
	{
		m_direction = m_direction.normalized();

		angle = m_direction.angle() + sf::degrees(270.0f);
	}
	else
	{
		return m_body.getRotation();
	}
	return angle;
}

bool Bird::collisionVisionConeTurtle(sf::Sprite& t_turtle)
{
	if (m_visionCone.getBounds().findIntersection(t_turtle.getGlobalBounds()))
	{
		m_state = BirdState::ATTACKING;
		m_chasingPoint = t_turtle.getPosition();
		m_movingFrom = m_body.getPosition();
		m_chasing = true;
		m_returning = false;
		m_killTimer.start();
		return true;
	}
	return false;
}

bool Bird::isPlayerDead()
{
	return m_playerDead;
}


