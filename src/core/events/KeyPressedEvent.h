#pragma once
#include "events/Event.h"

class KeyPressedEvent : public Event
{
public:
	SET_STATIC_TYPE(KEY_PRESSED);
	EventType inline GetType() { return EventType::KEY_PRESSED; }

	int Key;
    int Mods;
};
