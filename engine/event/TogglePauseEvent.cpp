#include "TogglePauseEvent.h"

TogglePauseEvent::TogglePauseEvent()
	: Event(EventType::TOGGLE_PAUSE)
{

}

TogglePauseEvent* TogglePauseEvent::Clone() const
{
	return new TogglePauseEvent(*this);
}
