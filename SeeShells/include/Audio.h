#pragma once

#include <SFML/Audio.hpp>

class Audio
// Interface that High level module (AudioSystem) and 
// Low level module (like ScreamAudio) depend on
{
public:
	virtual void play() = 0;
};

class FootstepAudio : public Audio
// Specific behaviour of low level module
{
public:
	void play() override;
private:
	sf::SoundBuffer m_footstepsBuffer{ "resources/AUDIOS/footsteps.wav" };
	sf::Sound m_footstepsSound{ m_footstepsBuffer };
};

class ScreamAudio : public Audio
// Specific behaviour of low level module
{
public:
	void play() override;
private:
	sf::SoundBuffer m_screamBuffer{ "resources/AUDIOS/scream.wav" };
	sf::Sound m_screamSound{ m_screamBuffer };
};

class WinAudio : public Audio
	// Specific behaviour of low level module
{
public:
	void play() override;
private:
	sf::SoundBuffer m_winBuffer{ "resources/AUDIOS/win.wav" };
	sf::Sound m_winSound{ m_winBuffer };
};