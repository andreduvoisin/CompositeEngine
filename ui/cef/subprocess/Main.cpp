#include "UIAppRenderer.h"
#include "UIAppOther.h"
#include "UIRenderProcessHandler.h"

#ifdef _WIN32
#include <windows.h>
#include "include/internal/cef_win.h"
#else
// TODO: CefMainArgs include on non-Windows operating systems.
#endif


enum class ProcessType
{
	BROWSER,
	RENDERER,
	ZYGOTE,
	OTHER
};


ProcessType ParseProcessType(CefRefPtr<CefCommandLine> commandLine)
{
	// The command-line flag won't be specified for the browser process.
	if (!commandLine->HasSwitch("type"))
	{
		return ProcessType::BROWSER;
	}
	
	CefString processType = commandLine->GetSwitchValue("type");

	if (processType == "renderer")
	{
		return ProcessType::RENDERER;
	}

#ifdef __linux__
	if (processType == "zygote")
	{
		return ProcessType::ZYGOTE;
	}
#endif

	return ProcessType::OTHER;
}


CefRefPtr<UIAppRenderer> CreateAppRenderer()
{
	CefMessageRouterConfig messageRouterConfig;
	CefRefPtr<CefMessageRouterRendererSide> messageRouterRendererSide = CefMessageRouterRendererSide::Create(messageRouterConfig);
	CefRefPtr<UIRenderProcessHandler> renderProcessHandler = new UIRenderProcessHandler(messageRouterRendererSide);
	return new UIAppRenderer(renderProcessHandler);
}


CefRefPtr<CefApp> CreateApp(CefRefPtr<CefCommandLine> commandLine)
{
	ProcessType processType = ParseProcessType(commandLine);

	if (processType == ProcessType::RENDERER)
	{
		return CreateAppRenderer();
	}

#ifdef __linux__
	if (processType == ProcessType::ZYGOTE)
	{
		// On Linux the zygote process is used to spawn other process types. Since
		// we don't know what type of process it will be give it the renderer
		// client.
		return CreateAppRenderer();
	}
#endif

	if (processType == ProcessType::OTHER)
	{
		return new UIAppOther();
	}

	return nullptr;
}


int main(int argc, char* argv[])
{
	CefMainArgs mainArgs;
	CefRefPtr<CefCommandLine> commandLine = CefCommandLine::CreateCommandLine();

#ifdef _WIN32
	mainArgs = CefMainArgs(::GetModuleHandle(NULL));
	commandLine->InitFromString(::GetCommandLineW());
#else
	mainArgs = CefMainArgs(argc, argv);
	commandLine->InitFromArgv(argc, argv);
#endif

	CefRefPtr<CefApp> app = CreateApp(commandLine);

	return CefExecuteProcess(mainArgs, app, nullptr);
}
