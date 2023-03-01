#include "Event.h"

Event::Event(Time event, int ID):eventTime(event)
{
	id = ID;
}

Time Event::getTime() const
{
	return eventTime;
}

int Event::getID() const
{
	return id;
}
