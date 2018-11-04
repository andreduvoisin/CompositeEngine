#include "PauseStateEvent.h"

PauseStateEvent::PauseStateEvent()
	: Event(EventId::PAUSE_STATE)
{

}

PauseStateEvent* PauseStateEvent::Clone() const
{
	return new PauseStateEvent(*this);
}

RequestPauseStateEvent::RequestPauseStateEvent()
	: Event(EventId::REQUEST_PAUSE_STATE)
{

}

RequestPauseStateEvent* RequestPauseStateEvent::Clone() const
{
	return new RequestPauseStateEvent(*this);
}
