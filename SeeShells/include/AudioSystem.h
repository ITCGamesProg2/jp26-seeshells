#pragma once

#include "Observer.h"
#include "Event.h"
#include <iostream>

class AudioSystem : public Observer {
public:

	void update(Event t_event) override;
};