#pragma once
#include "events/Event.h"

class WindowResizeEvent : public Event
{
public:
	SET_STATIC_TYPE(WINDOW_RESIZE);
	EventType inline GetType() { return EventType::WINDOW_RESIZE; }

	unsigned int Width, Height;
};