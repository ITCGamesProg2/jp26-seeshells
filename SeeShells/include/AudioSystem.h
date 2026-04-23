#pragma once

#include "Observer.h"
#include "Event.h"
#include <iostream>
#include "Audio.h"

class AudioSystem : public Observer {
public:
	/// <summary>
	/// Play soundFX for a corresponding event should it happen
	/// </summary>
	/// <param name="t_event"></param>
	void update(Event t_event) override;
private:
	Audio *m_audio;
};