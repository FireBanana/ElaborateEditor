#pragma once

#include <functional>

enum class EventType
{
	WINDOW_RESIZE,
	MOUSE_MOVED,
	KEY_PRESSED
};

#define SET_STATIC_TYPE(type) static EventType GetStaticType() { return EventType::type; }

class Event
{
public:

	virtual inline EventType GetType() = 0;

private:
};

class EventHandler
{
public:
	EventHandler(Event& event)
		: m_Event(event)
	{}

	template<typename T>
	void Dispatch(std::function<void()> const& cb)
	{
		if (m_Event.GetType() == T::GetStaticType())
		{
			cb();
		}
	}

private:
	Event& m_Event;
};