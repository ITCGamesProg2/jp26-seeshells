#pragma once

#include "Event.h"

class Observer 
{
public:
    virtual void update(Event t_event) = 0;
};

