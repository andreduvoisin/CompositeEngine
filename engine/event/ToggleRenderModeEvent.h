#ifndef _CE_TOGGLE_RENDER_MODE_EVENT_H_
#define _CE_TOGGLE_RENDER_MODE_EVENT_H_

#include "core/Event.h"

struct ToggleRenderModeEvent : Event
{
	ToggleRenderModeEvent();
	ToggleRenderModeEvent* Clone() const override;
};

#endif // _CE_TOGGLE_RENDER_MODE_EVENT_H_
