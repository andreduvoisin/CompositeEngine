#include "SdlEvent.h"

#include "common/json/JsonSerializer.h"
#include "common/json/JsonDeserializer.h"

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
