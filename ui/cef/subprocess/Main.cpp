#include "UIAppOther.h"
#include "UIAppRenderer.h"
#include "UIRenderProcessHandler.h"
#include "include/cef_base.h"

#ifdef __APPLE__
#    include "include/wrapper/cef_library_loader.h"
#endif

enum class ProcessType
{
    BROWSER,
    RENDERER,
    ZYGOTE,
    OTHER
};

ProcessType ParseProcessType(const CefRefPtr<CefCommandLine>& commandLine)
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
    CefRefPtr<CefMessageRouterRendererSide> messageRouterRendererSide =
            CefMessageRouterRendererSide::Create(messageRouterConfig);
    CefRefPtr<UIRenderProcessHandler> renderProcessHandler = new UIRenderProcessHandler(messageRouterRendererSide);
    return new UIAppRenderer(renderProcessHandler);
}

CefRefPtr<CefApp> CreateApp(const CefRefPtr<CefCommandLine>& commandLine)
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

// NOLINTNEXTLINE(misc-unused-parameters)
int main(int argc, char* argv[])
{
#ifdef __APPLE__
    CefScopedLibraryLoader libraryLoader;
    if (!libraryLoader.LoadInMain())
    {
        printf("CEF failed to load framework in subprocess.\n");
        return 1;
    }
#endif

    CefMainArgs mainArgs;
    CefRefPtr<CefCommandLine> commandLine = CefCommandLine::CreateCommandLine();

#ifdef _WIN32
    mainArgs = CefMainArgs(::GetModuleHandle(nullptr));
    commandLine->InitFromString(::GetCommandLineW());
#else
    mainArgs = CefMainArgs(argc, argv);
    commandLine->InitFromArgv(argc, argv);
#endif

    CefRefPtr<CefApp> app = CreateApp(commandLine);

    return CefExecuteProcess(mainArgs, app, nullptr);
}
