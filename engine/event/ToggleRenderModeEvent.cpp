#include "ToggleRenderModeEvent.h"

ToggleRenderModeEvent::ToggleRenderModeEvent()
	: Event(EventId::TOGGLE_RENDER_MODE)
{

}

ToggleRenderModeEvent* ToggleRenderModeEvent::Clone() const
{
	return new ToggleRenderModeEvent(*this);
}
