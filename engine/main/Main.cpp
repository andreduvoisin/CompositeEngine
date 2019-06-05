
#include "core/centralcore.h"
#include "common/debug/AssertThread.h"
#include "core/clock/RealTimeClock.h"
#include "core/clock/GameTimeClock.h"
#include "event/SdlEvent.h"
#include "core/EditorCameraEventHandler.h"

int main(int argc, char* argv[])
{
	CE_SET_MAIN_THREAD();

	if (!CE::core().initialize(argc, argv))
	{
		printf("Failed to initialize.\n");
		return 1;
	}

  return CE::core().runLoop();
}
