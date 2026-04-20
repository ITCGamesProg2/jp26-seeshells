#pragma once

#include "Observer.h"
#include "Event.h"

class AudioSystem : public Observer {
public:

	void update(Event t_event) override;
};