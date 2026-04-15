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
	m_chasingPoint = { 1000.0f,1000.0f };
}
void Bird::update(float dt)
{
	if (!m_rotated)
	{
		rotate();
	}
	if (m_moving)
	{
		move(dt);
	}
	visionCone();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X))
	{
		m_state = BirdState::SEARCHING;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C))
	{
		m_state = BirdState::ALERT;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::V))
	{
		m_state = BirdState::PURSUING;
		m_direction.x = m_chasingPoint.x - m_body.getPosition().x;
		m_direction.y = m_chasingPoint.y - m_body.getPosition().y;
		m_direction = m_direction.normalized();
		m_rotated = false;
		m_moving = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B))
	{
		m_state = BirdState::ATTACKING;
	}

}

void Bird::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_visionCone);
	t_window.draw(m_body);
}

void Bird::visionCone()
{
	sf::Vector2f spritePos = m_body.getPosition();
	sf::Color coneColor;
	
	switch (m_state)
	{
	case BirdState::SEARCHING:
		m_angleOfFan = 45.0f;
		m_reachOfFan = 100.0f;
		coneColor = sf::Color(0, 255, 0, 50);
		break;
	case BirdState::ALERT:
		m_angleOfFan = 35.0f;
		m_reachOfFan = 100.0f;
		coneColor = sf::Color(255, 255, 0, 50);
		break;
	case BirdState::PURSUING:
		m_angleOfFan = 25.0f;
		m_reachOfFan = 125.0f;
		coneColor = sf::Color(255, 165, 0, 50);
		break;
	case BirdState::ATTACKING:
		m_angleOfFan = 15.0f;
		m_reachOfFan = 150.0f;
		coneColor = sf::Color(255, 0, 0, 50);
		break;
	default:
		break;
	}

	m_visionCone[0].position = { spritePos.x, spritePos.y };
	m_visionCone[0].color = coneColor;

	for (int i = 1; i < 12; i++)
	{
		float relAngle = (i - 1) * (m_angleOfFan / 10) - (m_angleOfFan / 2.0f);

		float totalAngle = m_body.getRotation().asDegrees() + relAngle + 90.0f;

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
		newPos.x = m_body.getPosition().x + (m_direction.x * m_speed);
		newPos.y = m_body.getPosition().y + (m_direction.y * m_speed);

		//m_angle = std::atan2f(m_direction.y, m_direction.x);

		//m_body.setRotation(sf::radians(m_angle - 90.0f));
		m_body.setPosition(newPos);
		break;
	case BirdState::ATTACKING:
		break;
	default:
		break;
	}

	if (m_angle >= 360.0f)
	{
		m_angle = 0.0f;
	}
}

void Bird::collisionVisionConeScent(std::vector<Scent> t_playerScent)
{
	for (int i = 0; i < t_playerScent.size(); i++)
	{
		if (t_playerScent.at(i).m_circle.getGlobalBounds().findIntersection(m_visionCone.getBounds()))
		{
			if (i < 2)
			{
				m_state = BirdState::ALERT;
			}
			else
			{
				m_state = BirdState::PURSUING;
				m_chasingPoint = t_playerScent.at(i).m_circle.getPosition();
				m_direction.x = m_chasingPoint.x - m_body.getPosition().x;
				m_direction.y = m_chasingPoint.y - m_body.getPosition().y;
				m_direction = m_direction.normalized();
			}
		}
	}
}

void Bird::rotate()
{
	float cross;
	cross = std::cos(m_body.getRotation().asRadians()) * m_direction.y - std::sin(m_body.getRotation().asRadians()) * m_direction.x;
	float rotationFloat;

	if (std::abs(cross) < 0.01)
	{
		cross = 0.0;
		m_rotated = true;
		m_moving = true;
	}
	else if (cross > 0.01)
	{
		m_rotation += sf::degrees(1.0);
	}
	else
	{
		m_rotation -= sf::degrees(1.0);
	}
	m_body.setRotation(m_rotation + sf::radians(1.57f));
}
