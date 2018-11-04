#include "TogglePauseEvent.h"

TogglePauseEvent::TogglePauseEvent()
	: Event(EventId::TOGGLE_PAUSE)
{

}

TogglePauseEvent* TogglePauseEvent::Clone() const
{
	return new TogglePauseEvent(*this);
}
