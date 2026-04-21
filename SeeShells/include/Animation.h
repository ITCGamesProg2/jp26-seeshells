#pragma once

#include <SFML/Graphics.hpp>

class Animation
{
public:
	Animation();

	void addFrame(const sf::IntRect& frame);

	void update();
	sf::IntRect getCurrentFrame() const;

	void setFrameTime(int time);
	void setLooping(bool loop);
	bool isAnimationFinished() const;

private:
	std::vector<sf::IntRect> frames;
	size_t currentFrame;
	int frameTime;
	int elapsedTime;
	bool looping;
	bool isFinished = false;
};

