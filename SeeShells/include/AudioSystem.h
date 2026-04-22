#pragma once

#include "Observer.h"
#include "Event.h"
#include <iostream>
#include "AudioPlayer.h"

class AudioSystem : public Observer {
public:

	void update(Event t_event) override;

private:
	AudioPlayer m_audioPlayer;
};