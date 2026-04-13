#include "Bird.h"

Bird::Bird(AssetManager& t_assetManager, sf::Vector2f t_pos):
	m_assetManager(t_assetManager),
	m_body(t_assetManager.getTexture("bird")),
	m_center(t_pos)
{
	m_body.setPosition({ 600.0f,600.0f });
	m_body.setOrigin({ 16.0f,16.0f });
	m_angle = 0.0f;
}
void Bird::update(float dt)
{
	move(dt);
}

void Bird::render(sf::RenderWindow& t_window)
{
	t_window.draw(visionCone());
	t_window.draw(m_body);
}

sf::VertexArray Bird::visionCone()
{
	sf::Vector2f spritePos = m_body.getPosition();
	sf::VertexArray visionCone(sf::PrimitiveType::TriangleFan, 12);
	sf::Color coneColor;
	
	switch (m_state)
	{
	case BirdState::SEARCHING:
		coneColor = sf::Color(0, 255, 0, 50);
		break;
	case BirdState::ALERT:
		coneColor = sf::Color(255, 255, 0, 50);
	case BirdState::PURSUING:
		coneColor = sf::Color(255, 165, 0, 50);
		break;
	case BirdState::ATTACKING:
		coneColor = sf::Color(255, 0, 0, 50);
		break;
	default:
		break;
	}

	visionCone[0].position = { spritePos.x, spritePos.y };
	visionCone[0].color = coneColor;

	for (int i = 1; i < 12; i++)
	{
		float relAngle = (i - 1) * (45.0f / 10) - (45.0f / 2.0f);

		float totalAngle = m_body.getRotation().asDegrees() + relAngle + 90.0f;

		float radAngle = totalAngle * (3.14159f / 180.f);

		visionCone[i].position = { spritePos.x + std::cos(radAngle) * 100, spritePos.y + std::sin(radAngle) * 100 };
		visionCone[i].color = coneColor;
	}

	return visionCone;
}

void Bird::move(float dt)
{
	if (m_angle >= 360.0f)
	{
		m_angle = 0.0f;
	}
	sf::Vector2f newPos;
	m_angle += (m_speed * dt/1000);

	newPos.x = m_center.x + m_radius * std::cos(m_angle);
	newPos.y = m_center.y + m_radius * std::sin(m_angle);

	m_body.setRotation(sf::radians(m_angle));
	m_body.setPosition(newPos);
}
