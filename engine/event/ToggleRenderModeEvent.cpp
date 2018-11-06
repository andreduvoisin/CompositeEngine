#include "ToggleRenderModeEvent.h"

ToggleRenderModeEvent::ToggleRenderModeEvent()
	: Event(EventType::TOGGLE_RENDER_MODE)
{

}

ToggleRenderModeEvent* ToggleRenderModeEvent::Clone() const
{
	return new ToggleRenderModeEvent(*this);
}
