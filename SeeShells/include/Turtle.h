#pragma once

#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "Animation.h"
#include "Subject.h"
#include "CollisionDetector.h"
#include "Drawable.h"

struct Scent //Player trail
{
	sf::CircleShape m_circle;
	sf::Clock m_timer;
};

class Turtle : public Subject, public Drawable
{
private:

	float m_speed{ 0.0f };

	sf::Sprite m_body; // Turtle sprite

	AssetManager& m_assetManager;

	std::vector<Scent> m_scentTrail; // Holds scent trail 
	sf::Clock m_scentTimer; // How often should scent trail circles spawn and disappear

	Animation m_moveAnimation;
	Animation m_hideAnimation;
	Animation m_shellOnAnimation;
	Animation m_shellOffAnimation;
	Animation* m_currAnimation;

	sf::Angle m_rotation{sf::degrees(0.0f)}; // Rotates turtle

	bool m_isHiding = false; // Checks if turtle is hiding

	/// <summary>
	/// Handles turtle movement
	/// </summary>
	/// <param name="t_dt"> delta time </param>
	void move(float t_dt); 

	sf::RectangleShape m_winRect; // Where turtle wins
	bool m_didWin = false; // Checks if turtle won

public:

	Turtle(AssetManager &t_assetManager, std::vector<sf::Sprite>& t_obstacleSprites);
	
	enum class CollisionState { NORMAL, COLLIDING };
	CollisionState m_state{ CollisionState::NORMAL };

	enum class AnimationState { MOVE, SHELL_ON, SHELL_OFF, HIDE };
	AnimationState m_animationState{ AnimationState::MOVE };
	sf::Clock m_animationDelay;

	std::vector<sf::Sprite>& m_obstacleSprites;
	sf::Vector2f m_contactNormal;
	//void init();

	void update(float t_dt);

	void render(sf::RenderWindow& t_window) override;
	void manageScent();
	std::vector<Scent> getScent();
	sf::Sprite getSprite();
	void handleMoveInput();
	void handleHideInput();
	void increaseSpeed();
	void increaseRotation();
	void decreaseSpeed();
	void decreaseRotation();
	bool isPlayerHiding();

	bool checkCollision();
	void deflect(float t_dt);

	void processAnimation();
	void initAnimation();

	void reset();
	void checkWin();
	bool getWin();

};

