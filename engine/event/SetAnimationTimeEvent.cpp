#include "SetAnimationTimeEvent.h"

SetAnimationTimeEvent::SetAnimationTimeEvent()
	: Event(EventType::SET_ANIMATION_TIME)
{

}

SetAnimationTimeEvent* SetAnimationTimeEvent::Clone() const
{
	return new SetAnimationTimeEvent(*this);
}
