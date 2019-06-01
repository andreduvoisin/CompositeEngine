#ifndef _CE_SDL_EVENT_H_
#define _CE_SDL_EVENT_H_

#include "core/Event.h"

#include <SDL_events.h>

namespace CE
{
    struct SdlEvent : Event
    {
        SdlEvent();
        SdlEvent* Clone() const override;

        SDL_Event event;

    protected:
        void SerializeInternal(JsonSerializer& serializer) const override;
        void DeserializeInternal(const JsonDeserializer& deserializer) override;
    };
}

#endif // _CE_SDL_EVENT_H_