#pragma once

#include <SFML/Audio.hpp>

class AudioPlayer
{
public:
	void playScream();
	void playFootsteps();

private:
	sf::SoundBuffer m_screamBuffer{ "resources/AUDIOS/scream.wav" };
	sf::Sound m_screamSound{ m_screamBuffer };

	sf::SoundBuffer m_footstepsBuffer{ "resources/AUDIOS/footsteps.wav" };
	sf::Sound m_footstepsSound{ m_footstepsBuffer };

};