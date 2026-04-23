#pragma once

#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "Animation.h"
#include "Drawable.h"

struct Scent;

enum class BirdState //used for tracking the state of the bird
{
	SEARCHING,
	ALERT,
	PURSUING,
	ATTACKING
};

class Bird : public Drawable
{
public:
	Bird(AssetManager& t_assetManager, sf::Vector2f pos);
	/// <summary>
	/// Updates bird behaviour
	/// </summary>
	/// <param name="dt"> delta time</param>
	/// <param name="t_playerScent"> used for tracking player trail</param>
	/// <param name="t_turtle"> player </param>
	/// <param name="t_isHiding"> is the player hiding? </param>
	void update(float dt, std::vector<Scent>& t_playerScent, sf::Sprite& t_turtle, bool t_isHiding);

	/// <summary>
	/// Draws everything bird related
	/// </summary>
	/// <param name="t_window"> render window </param>
	void render(sf::RenderWindow& t_window) override;

	/// <summary>
	/// Moves bird, based on bird state
	/// </summary>
	/// <param name="dt"> delta time </param>
	void move(float dt);

	/// <summary>
	/// Handles collision between birds vision cone and player trail
	/// </summary>
	/// <param name="t_playerScent"> player trail </param>
	void collisionVisionConeScent(std::vector<Scent> &t_playerScent);

	/// <summary>
	/// Rotates bird when PURSUING/ATTACKING
	/// </summary>
	/// <param name="t_pointToLookAt"> where should bird be looking </param>
	/// <returns> angle of rotation of bird</returns>
	sf::Angle lookAt(sf::Vector2f  t_pointToLookAt);

	/// <summary>
	/// Handles collision between birds vision cone and player
	/// </summary>
	/// <param name="t_turtle"> player </param>
	/// <returns> true if colliding, false if not </returns>
	bool collisionVisionConeTurtle(sf::Sprite& t_turtle);

	/// <summary>
	/// Returns player alive/dead status
	/// </summary>
	/// <returns> true if player was killed, false if not </returns>
	bool isPlayerDead();
	
private:
	/// <summary>
	/// Handles vision cone
	/// </summary>
	void visionCone();

	sf::Sprite m_body; // Sprite of bird
	AssetManager &m_assetManager;

	// Bird move variables (states SEARCHING,ALERT)
	sf::Vector2f m_center; // Center around which bird rotates
	float m_radius = 200.0f; // Radius of circle
	float m_angle; // Angle of rotation used for movement
	float m_speed = 0.5f; // Speed

	BirdState m_state = BirdState::SEARCHING; // State of bird

	// Vision cone variables
	sf::VertexArray m_visionCone; // Vision cone
	float m_angleOfFan; // How spread is the fan
	float m_reachOfFan; // How far the fan reaches

	// Move variables (states PURSUING,ATTACKING)
	sf::Vector2f m_chasingPoint; // Position where the bird moves
	sf::Vector2f m_direction; // Direction of movement
	sf::Vector2f m_movingFrom; // Position from which bird moved
	bool m_chasing = false; // Checks if bird is chasing
	bool m_returning = false; // Checks if bird is returning

	sf::Clock m_killTimer; // Timer between bird first saw player and bird kills player
	bool m_playerDead = false; // Is player dead?

	float m_ignoreScentTimer = 0.0f; // Ignores scent after moving to the chasing point
	float m_alertTimer = 0.0f; // Used for switching from ALERT to SEARCHING

	float m_diffUpdating;
	Animation m_moveAnimation;
	Animation* m_currAnimation;

};

#include "Turtle.h"
