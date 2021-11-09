#pragma once
#include "events/Event.h"

class MousePositionEvent : public Event
{
public:
	SET_STATIC_TYPE(MOUSE_POSITION);
	EventType inline GetType() { return EventType::MOUSE_POSITION; }

	double XPosition;
	double YPosition;
};
#pragma once
