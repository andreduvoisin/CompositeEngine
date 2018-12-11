#include "UIAppRender.h"
#include "UIRenderProcessHandler.h"

#include "include/cef_command_line.h"

#ifdef _WIN32
#include <windows.h>
#include "include/internal/cef_win.h"
#else
// TODO: Need CefMainArgs include on non-Windows operating systems.
#endif


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

	CefRefPtr<CefApp> app;

	// The command-line flag won't be specified for the browser process.
	if (!commandLine->HasSwitch("type"))
	{
		assert(false);
	}

	const std::string& processType = commandLine->GetSwitchValue("type");
	if (processType == "renderer" || processType == "zygote")
	{
		CefMessageRouterConfig messageRouterConfig;
		CefRefPtr<CefMessageRouterRendererSide> messageRouterRendererSide = CefMessageRouterRendererSide::Create(messageRouterConfig);
		CefRefPtr<UIRenderProcessHandler> renderProcessHandler = new UIRenderProcessHandler(messageRouterRendererSide);
		app = new UIAppRender(renderProcessHandler);
	}

	return CefExecuteProcess(mainArgs, app, nullptr);
}
