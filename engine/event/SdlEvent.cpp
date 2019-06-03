#include "SdlEvent.h"

namespace CE
{
    SdlEvent::SdlEvent()
        : Event(EventType::SDL)
    {

    }

    SdlEvent* SdlEvent::Clone() const
    {
        return new SdlEvent(*this);
    }
}
