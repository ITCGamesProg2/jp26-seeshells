#include "AudioSystem.h"


void AudioSystem::update(Event t_event)
{
	switch (t_event)
	{
	case Event::MOVE:
		m_audio = new FootstepAudio;
		m_audio->play();
		break;
	case Event::DIE:
		m_audio = new ScreamAudio;
		m_audio->play();
		break;
	case Event::WIN:
		m_audio = new WinAudio;
		m_audio->play();
		break;
	}
}
