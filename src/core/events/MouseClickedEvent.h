#pragma once
#include "events/Event.h"

class MouseClickedEvent : public Event
{
public:
	SET_STATIC_TYPE(MOUSE_CLICKED);
	EventType inline GetType() { return EventType::MOUSE_CLICKED; }

	int Button;
	int Action;
	int Mods;
};
