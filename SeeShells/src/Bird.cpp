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
}
void Bird::update(float dt, std::vector<Scent> &t_playerScent)
{
	collisionVisionConeScent(t_playerScent);
	move(dt);
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
	float targetAngleDeg;
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
		targetAngleDeg = std::atan2(m_direction.y, m_direction.x) * (180.0f / 3.14159f);
		targetAngleDeg = targetAngleDeg * (3.14159f / 180.0f);
		newPos.x = m_body.getPosition().x + std::cos(targetAngleDeg) * m_speed * (dt / 1000.0f);
		newPos.y = m_body.getPosition().y + std::sin(targetAngleDeg) * m_speed * (dt / 1000.0f);

		m_body.setPosition(newPos);
		m_body.setRotation(sf::degrees(targetAngleDeg + 90.0f));

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

void Bird::collisionVisionConeScent(std::vector<Scent> &t_playerScent)
{
	sf::Vector2f birdPos = m_body.getPosition();
	for (int i = t_playerScent.size() - 1; i >= 0; i--)
	{
		if (t_playerScent.at(i).m_circle.getGlobalBounds().findIntersection(m_visionCone.getBounds()))
		{
			m_state = BirdState::PURSUING;
			m_chasingPoint = t_playerScent.at(i).m_circle.getPosition();
			m_direction = m_chasingPoint - birdPos;
			m_direction = m_direction.normalized();
			t_playerScent.erase(t_playerScent.begin() + i);
		}
	}
}