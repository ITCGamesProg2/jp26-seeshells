#include "AudioSystem.h"


void AudioSystem::update(Event t_event)
{
	switch (t_event)
	{
	case Event::MOVE:
		//std::cout << "Moving SFX is playing\n";
		break;
	case Event::BUTTON_PRESS:
		//std::cout << "Button Press SFX is playing\n";
		break;
	case Event::DIE:
		//std::cout << "Dying SFX is playing\n";
		break;
	}
}
