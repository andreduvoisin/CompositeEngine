#include "UIBrowserProcessHandler.h"

#include "UIExternalMessagePump.h"

UIBrowserProcessHandler::UIBrowserProcessHandler(UIExternalMessagePump* externalMessagePump)
    : externalMessagePump(externalMessagePump)
{
}

void UIBrowserProcessHandler::OnScheduleMessagePumpWork(int64 delayMillis)
{
    uint32_t delayMillisUint32 = ClampToUint32(delayMillis);
    externalMessagePump->OnScheduleMessagePumpWork(delayMillisUint32);
}

uint32_t UIBrowserProcessHandler::ClampToUint32(int64 value)
{
    value = std::clamp(
            value,
            static_cast<int64>((std::numeric_limits<uint32_t>::min)()),
            static_cast<int64>((std::numeric_limits<uint32_t>::max)()));
    return static_cast<uint32_t>(value);
}
