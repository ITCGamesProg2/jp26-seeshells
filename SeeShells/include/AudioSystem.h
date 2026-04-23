#pragma once

#include "Observer.h"
#include "Event.h"
#include <iostream>
#include "Audio.h"

class AudioSystem : public Observer {
public:
	void update(Event t_event) override;
private:
	Audio *m_audio;
};