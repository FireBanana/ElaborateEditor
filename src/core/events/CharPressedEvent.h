#pragma once
#include "events/Event.h"

class CharPressedEvent : public Event
{
public:
	SET_STATIC_TYPE(CHAR_PRESSED);
	EventType inline GetType() { return EventType::CHAR_PRESSED; }

	unsigned int Key;
};
