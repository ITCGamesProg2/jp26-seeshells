#include "Animation.h"


Animation::Animation() : currentFrame(0), frameTime(10), elapsedTime(0.0f), looping(true) {}

void Animation::addFrame(const sf::IntRect& frame) 
{
	frames.push_back(frame);
}

void Animation::update() 
{
	if (frames.empty()) return;

	elapsedTime += 1;
	if (elapsedTime >= frameTime) {
		elapsedTime = 0;
		currentFrame = (currentFrame + 1) % frames.size();
		if (!looping && currentFrame == frames.size() - 1) {
			// Stop at the last frame if not looping
			isFinished = true;
			resetCurrentFrame();
		}
	}
}

sf::IntRect Animation::getCurrentFrame() const 
{
	if (frames.empty()) return sf::IntRect();
	return frames[currentFrame];
}

void Animation::resetCurrentFrame()
{
	currentFrame = 0;
}

void Animation::setFrameTime(int time) 
{ 
	frameTime = time; 
}


void Animation::setLooping(bool loop)
{
	looping = loop; 
}


bool Animation::isAnimationFinished() const 
{ 
	return isFinished; 
}

