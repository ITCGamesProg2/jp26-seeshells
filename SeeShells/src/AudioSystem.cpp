#include "AudioSystem.h"


void AudioSystem::update(Event t_event)
{
	switch (t_event)
	{
	case Event::MOVE:
		m_audioPlayer.playFootsteps();
		break;
	case Event::DIE:
		m_audioPlayer.playScream();
		break;
	}
}
