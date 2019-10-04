#include "CefMain.h"

#include "cef/browser/UIAppBrowser.h"
#include "cef/browser/UIBrowserProcessHandler.h"
#include "cef/browser/UIExternalMessagePump.h"
#include "cef/browser/UIQueryHandler.h"
#include "cef/browser/UIQueryResponder.h"
#include "cef/client/UIClient.h"
#include "cef/client/UILifeSpanHandler.h"
#include "cef/client/UIRenderHandler.h"
#include "cef/client/UIRequestHandler.h"
#include "event/SdlEvent.h"
#include "event/WindowsMessageEvent.h"
#include "event/core/EventSystem.h"
#include "include/cef_app.h"
#include "include/wrapper/cef_message_router.h"
#include <SDL_syswm.h>

#ifdef __APPLE__
#    include "cef/WindowContentView.h"
#    include "include/wrapper/cef_library_loader.h"
#    include <CoreFoundation/CoreFoundation.h>
#endif

namespace CE
{
    CefMain::CefMain(EventSystem* eventSystem, SDL_Window* window)
        : eventSystem(eventSystem)
        , window(window)
    {
        eventSystem->RegisterListener(this, EventType::SDL);
        eventSystem->RegisterListener(this, EventType::WINDOWS_MESSAGE);
    }

    bool CefMain::StartCef(int argc, char* argv[], uint32_t screenWidth, uint32_t screenHeight)
    {
#ifdef __APPLE__
        CefScopedLibraryLoader libraryLoader;
        if (!libraryLoader.LoadInMain())
        {
            printf("CEF failed to load framework in engine.\n");
            return false;
        }
#endif

        CefMainArgs mainArgs;
#ifdef _WIN32
        mainArgs = CefMainArgs(::GetModuleHandle(NULL));
#elif __APPLE__
        mainArgs = CefMainArgs(argc, argv);
#endif

        queryHandler = new UIQueryHandler(eventSystem, new UIQueryResponder(eventSystem));
        externalMessagePump = new UIExternalMessagePump();
        CefRefPtr<UIBrowserProcessHandler> browserProcessHandler = new UIBrowserProcessHandler(externalMessagePump);
        CefRefPtr<UIAppBrowser> app = new UIAppBrowser(browserProcessHandler);

        CefSettings settings;
        settings.no_sandbox = 1;
        settings.external_message_pump = 1;
        settings.windowless_rendering_enabled = 1;
        settings.remote_debugging_port = 3469;
#ifdef _WIN32
        CefString(&settings.browser_subprocess_path).FromASCII("CompositeCefSubprocess.exe");
#elif __APPLE__
        CFBundleRef mainBundle = CFBundleGetMainBundle();
        // TODO: Free? See
        // https://developer.apple.com/library/archive/documentation/CoreFoundation/Conceptual/CFMemoryMgmt/Concepts/Ownership.html#//apple_ref/doc/uid/20001148-103029
        CFURLRef privateFrameworksUrl = CFBundleCopyPrivateFrameworksURL(mainBundle);
        UInt8 privateFrameworksDirectoryName[1024];
        CFURLGetFileSystemRepresentation(privateFrameworksUrl, 1U, privateFrameworksDirectoryName, 1024);

        std::string subprocessFile = (char*) privateFrameworksDirectoryName;
        subprocessFile += "/CompositeCefSubprocess.app/Contents/MacOS/CompositeCefSubprocess";
        CefString(&settings.browser_subprocess_path).FromString(subprocessFile);
#endif

        if (!CefInitialize(mainArgs, settings, app, nullptr))
        {
            printf("CEF failed to initialize.\n");
            return false;
        }

        CefMessageRouterConfig messageRouterConfig;
        CefRefPtr<CefMessageRouterBrowserSide> messageRouterBrowserSide =
                CefMessageRouterBrowserSide::Create(messageRouterConfig);
        messageRouterBrowserSide->AddHandler(queryHandler, true);

        CefRefPtr<UIContextMenuHandler> contextMenuHandler = new UIContextMenuHandler();
        CefRefPtr<UIRenderHandler> renderHandler = new UIRenderHandler(screenWidth, screenHeight);
        CefRefPtr<UILifeSpanHandler> lifeSpanHandler = new UILifeSpanHandler(messageRouterBrowserSide);
        CefRefPtr<UILoadHandler> loadHandler = new UILoadHandler();
        CefRefPtr<UIRequestHandler> requestHandler = new UIRequestHandler(messageRouterBrowserSide);
        uiClient = new UIClient(
                contextMenuHandler,
                renderHandler,
                lifeSpanHandler,
                loadHandler,
                requestHandler,
                messageRouterBrowserSide);

        SDL_SysWMinfo sysInfo;
        SDL_VERSION(&sysInfo.version);
        if (SDL_GetWindowWMInfo(window, &sysInfo) == 0U)
        {
            return false;
        }

        CefBrowserSettings browserSettings;
        CefWindowInfo windowInfo;
#ifdef _WIN32
        windowInfo.SetAsWindowless(sysInfo.info.win.window);
#elif __APPLE__
        auto* view = (NSView*) CE::GetWindowContentView(sysInfo.info.cocoa.window);
        windowInfo.SetAsWindowless(view);
#endif

        browser = CefBrowserHost::CreateBrowserSync(
                windowInfo,
                uiClient,
                "http://localhost:3000", // "about:blank"
                browserSettings,
                nullptr);

        return true;
    }

    void CefMain::StopCef()
    {
        externalMessagePump->Shutdown();

        browser->GetHost()->CloseBrowser(true);

        browser = nullptr;
        uiClient = nullptr;

        CefShutdown();
    }

    const char* CefMain::GetViewBuffer()
    {
        return (dynamic_cast<UIRenderHandler*>(uiClient->GetRenderHandler().get()))->GetViewBuffer();
    }

    const char* CefMain::GetPopupBuffer()
    {
        return (dynamic_cast<UIRenderHandler*>(uiClient->GetRenderHandler().get()))->GetPopupBuffer();
    }

    const CefRect& CefMain::GetPopupRect()
    {
        return (dynamic_cast<UIRenderHandler*>(uiClient->GetRenderHandler().get()))->GetPopupRect();
    }

    void CefMain::OnEvent(const Event& event)
    {
        switch (event.type)
        {
            case EventType::SDL:
            {
                HandleSdlEvent(event);
                break;
            }
#ifdef _WIN32
            case EventType::WINDOWS_MESSAGE:
            {
                HandleWindowsMessageEvent(event);
                break;
            }
#endif
        }
    }

    void CefMain::HandleSdlEvent(const Event& event)
    {
        const auto& wrappedEvent = reinterpret_cast<const SdlEvent&>(event);

        const SDL_Event& nativeEvent = wrappedEvent.event;

        externalMessagePump->ProcessEvent(nativeEvent);

        switch (nativeEvent.type)
        {
            case SDL_KEYDOWN:
            {
#ifdef _WIN32
                switch (nativeEvent.key.keysym.sym)
                {
                    case SDLK_F11:
                    case SDLK_F12:
                    {
                        ToggleDevToolsWindow();
                        break;
                    }
                }
#endif
                break;
            }

            // osr_window_win.cc
            // browser_window_osr_mac.mm
            case SDL_MOUSEMOTION:
            {
                // TODO: CEF also gets all of the right-click camera movement events, which are unnecessary.
                CefMouseEvent mouseEvent;
                mouseEvent.x = nativeEvent.motion.x;
                mouseEvent.y = nativeEvent.motion.y;
                mouseEvent.modifiers = GetSdlCefInputModifiers(nativeEvent);

                browser->GetHost()->SendMouseMoveEvent(mouseEvent, false);

                break;
            }

            // osr_window_win.cc
            // browser_window_osr_mac.mm
            case SDL_MOUSEBUTTONDOWN:
            {
                CefBrowserHost::MouseButtonType mouseButtonType;
                if (nativeEvent.button.button == SDL_BUTTON_LEFT)
                {
                    mouseButtonType = MBT_LEFT;
                }
                else if (nativeEvent.button.button == SDL_BUTTON_MIDDLE)
                {
                    mouseButtonType = MBT_MIDDLE;
                }
                else if (nativeEvent.button.button == SDL_BUTTON_RIGHT)
                {
                    mouseButtonType = MBT_RIGHT;
                }
                else
                {
                    break;
                }

                CefMouseEvent mouseEvent;
                mouseEvent.x = nativeEvent.button.x;
                mouseEvent.y = nativeEvent.button.y;
                mouseEvent.modifiers = GetSdlCefInputModifiers(nativeEvent);

                browser->GetHost()->SendMouseClickEvent(mouseEvent, mouseButtonType, false, nativeEvent.button.clicks);

                break;
            }

            // osr_window_win.cc
            // browser_window_osr_mac.mm
            case SDL_MOUSEBUTTONUP:
            {
                CefBrowserHost::MouseButtonType mouseButtonType;
                if (nativeEvent.button.button == SDL_BUTTON_LEFT)
                {
                    mouseButtonType = MBT_LEFT;
                }
                else if (nativeEvent.button.button == SDL_BUTTON_MIDDLE)
                {
                    mouseButtonType = MBT_MIDDLE;
                }
                else if (nativeEvent.button.button == SDL_BUTTON_RIGHT)
                {
                    mouseButtonType = MBT_RIGHT;
                }
                else
                {
                    break;
                }

                CefMouseEvent mouseEvent;
                mouseEvent.x = nativeEvent.button.x;
                mouseEvent.y = nativeEvent.button.y;
                mouseEvent.modifiers = GetSdlCefInputModifiers(nativeEvent);

                browser->GetHost()->SendMouseClickEvent(mouseEvent, mouseButtonType, true, nativeEvent.button.clicks);

                break;
            }

            // osr_window_win.cc
            // browser_window_osr_mac.mm
            case SDL_MOUSEWHEEL:
            {
                CefMouseEvent mouseEvent;
                SDL_GetMouseState(&mouseEvent.x, &mouseEvent.y);
                mouseEvent.modifiers = GetSdlCefInputModifiers(nativeEvent);

                browser->GetHost()->SendMouseWheelEvent(mouseEvent, nativeEvent.wheel.x, nativeEvent.wheel.y);

                break;
            }

            // osr_window_win.cc
            // browser_window_osr_mac.mm
            case SDL_WINDOWEVENT:
            {
                switch (nativeEvent.window.event)
                {
                    case SDL_WINDOWEVENT_LEAVE:
                    {
                        CefMouseEvent mouseEvent;
                        SDL_GetMouseState(&mouseEvent.x, &mouseEvent.y);
                        mouseEvent.modifiers = GetSdlCefInputModifiers(nativeEvent);

                        browser->GetHost()->SendMouseMoveEvent(mouseEvent, true);

                        break;
                    }
                }

                break;
            }
        }
    }

    // osr_window_win.cc
    // browser_window_osr_mac.mm
    unsigned CefMain::GetSdlCefInputModifiers(const SDL_Event& event)
    {
        unsigned modifiers = 0;

        SDL_Keymod keymod = SDL_GetModState();

        if ((keymod & KMOD_CTRL) != 0)
        {
            modifiers |= EVENTFLAG_CONTROL_DOWN;
        }

        if ((keymod & KMOD_SHIFT) != 0)
        {
            modifiers |= EVENTFLAG_SHIFT_DOWN;
        }

        if ((keymod & KMOD_ALT) != 0)
        {
            modifiers |= EVENTFLAG_ALT_DOWN;
        }

        if ((keymod & KMOD_NUM) != 0)
        {
            modifiers |= EVENTFLAG_NUM_LOCK_ON;
        }

        if ((keymod & KMOD_CAPS) != 0)
        {
            modifiers |= EVENTFLAG_CAPS_LOCK_ON;
        }

#ifdef __APPLE__
        if ((keymod & KMOD_GUI) != 0)
        {
            modifiers |= EVENTFLAG_COMMAND_DOWN;
        }
#endif

        // todo: if mouse-only, still keep these two if's?
        if (((keymod & KMOD_LSHIFT) != 0) || ((keymod & KMOD_LCTRL) != 0) || ((keymod & KMOD_LALT) != 0)
            || ((keymod & KMOD_LGUI) != 0))
        {
            modifiers |= EVENTFLAG_IS_LEFT;
        }

        if (((keymod & KMOD_RSHIFT) != 0) || ((keymod & KMOD_RCTRL) != 0) || ((keymod & KMOD_RALT) != 0)
            || ((keymod & KMOD_RGUI) != 0))
        {
            modifiers |= EVENTFLAG_IS_RIGHT;
        }

        switch (event.type)
        {
            // todo: remove and make this function mouse-only?
            case SDL_KEYDOWN:
            case SDL_KEYUP:
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_KP_DIVIDE:
                    case SDLK_KP_MULTIPLY:
                    case SDLK_KP_MINUS:
                    case SDLK_KP_PLUS:
                    case SDLK_KP_ENTER:
                    case SDLK_KP_1:
                    case SDLK_KP_2:
                    case SDLK_KP_3:
                    case SDLK_KP_4:
                    case SDLK_KP_5:
                    case SDLK_KP_6:
                    case SDLK_KP_7:
                    case SDLK_KP_8:
                    case SDLK_KP_9:
                    case SDLK_KP_0:
                    case SDLK_KP_PERIOD:
                    case SDLK_KP_EQUALS:
                    case SDLK_KP_COMMA:
                    case SDLK_KP_EQUALSAS400:
                    case SDLK_KP_00:
                    case SDLK_KP_000:
                    case SDLK_KP_LEFTPAREN:
                    case SDLK_KP_RIGHTPAREN:
                    case SDLK_KP_LEFTBRACE:
                    case SDLK_KP_RIGHTBRACE:
                    case SDLK_KP_TAB:
                    case SDLK_KP_BACKSPACE:
                    case SDLK_KP_A:
                    case SDLK_KP_B:
                    case SDLK_KP_C:
                    case SDLK_KP_D:
                    case SDLK_KP_E:
                    case SDLK_KP_F:
                    case SDLK_KP_XOR:
                    case SDLK_KP_POWER:
                    case SDLK_KP_PERCENT:
                    case SDLK_KP_LESS:
                    case SDLK_KP_GREATER:
                    case SDLK_KP_AMPERSAND:
                    case SDLK_KP_DBLAMPERSAND:
                    case SDLK_KP_VERTICALBAR:
                    case SDLK_KP_DBLVERTICALBAR:
                    case SDLK_KP_COLON:
                    case SDLK_KP_HASH:
                    case SDLK_KP_SPACE:
                    case SDLK_KP_AT:
                    case SDLK_KP_EXCLAM:
                    case SDLK_KP_MEMSTORE:
                    case SDLK_KP_MEMRECALL:
                    case SDLK_KP_MEMCLEAR:
                    case SDLK_KP_MEMADD:
                    case SDLK_KP_MEMSUBTRACT:
                    case SDLK_KP_MEMMULTIPLY:
                    case SDLK_KP_MEMDIVIDE:
                    case SDLK_KP_PLUSMINUS:
                    case SDLK_KP_CLEAR:
                    case SDLK_KP_CLEARENTRY:
                    case SDLK_KP_BINARY:
                    case SDLK_KP_OCTAL:
                    case SDLK_KP_DECIMAL:
                    case SDLK_KP_HEXADECIMAL:
                    {
                        modifiers |= EVENTFLAG_IS_KEY_PAD;
                        break;
                    }
                }

                break;
            }

            case SDL_MOUSEMOTION:
            {
                if ((event.motion.state & SDL_BUTTON_LMASK) != 0U)
                {
                    modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
                }

                if ((event.motion.state & SDL_BUTTON_MMASK) != 0U)
                {
                    modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
                }

                if ((event.motion.state & SDL_BUTTON_RMASK) != 0U)
                {
                    modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
                }

                break;
            }

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
                }

                if (event.button.button == SDL_BUTTON_MIDDLE)
                {
                    modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
                }

                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
                }

                break;
            }

            case SDL_MOUSEWHEEL:
            {
                unsigned state = SDL_GetMouseState(nullptr, nullptr);

                if ((state & SDL_BUTTON_LMASK) != 0U)
                {
                    modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
                }

                if ((state & SDL_BUTTON_MMASK) != 0U)
                {
                    modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
                }

                if ((state & SDL_BUTTON_RMASK) != 0U)
                {
                    modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
                }

                break;
            }

            case SDL_WINDOWEVENT:
            {
                switch (event.window.event)
                {
                    case SDL_WINDOWEVENT_LEAVE:
                    {
                        unsigned state = SDL_GetMouseState(nullptr, nullptr);

                        if ((state & SDL_BUTTON_LMASK) != 0U)
                        {
                            modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
                        }

                        if ((state & SDL_BUTTON_MMASK) != 0U)
                        {
                            modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
                        }

                        if ((state & SDL_BUTTON_RMASK) != 0U)
                        {
                            modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
                        }

                        break;
                    }
                }

                break;
            }
        }

        return modifiers;
    }

#ifdef _WIN32
    void CefMain::HandleWindowsMessageEvent(const Event& event)
    {
        const WindowsMessageEvent& windowsMessageEvent = reinterpret_cast<const WindowsMessageEvent&>(event);

        switch (windowsMessageEvent.message)
        {
            case WM_SYSCHAR:
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP:
            case WM_CHAR:
            {
                CefKeyEvent keyEvent;
                keyEvent.windows_key_code = static_cast<int>(windowsMessageEvent.wParam);
                keyEvent.native_key_code = static_cast<int>(windowsMessageEvent.lParam);
                keyEvent.is_system_key = windowsMessageEvent.message == WM_SYSCHAR
                        || windowsMessageEvent.message == WM_SYSKEYDOWN || windowsMessageEvent.message == WM_SYSKEYUP;
                if (windowsMessageEvent.message == WM_KEYDOWN || windowsMessageEvent.message == WM_SYSKEYDOWN)
                {
                    keyEvent.type = KEYEVENT_RAWKEYDOWN;
                }
                else if (windowsMessageEvent.message == WM_KEYUP || windowsMessageEvent.message == WM_SYSKEYUP)
                {
                    keyEvent.type = KEYEVENT_KEYUP;
                }
                else
                {
                    keyEvent.type = KEYEVENT_CHAR;
                }
                keyEvent.modifiers = GetNativeCefKeyboardModifiers(
                        static_cast<WPARAM>(windowsMessageEvent.wParam),
                        static_cast<LPARAM>(windowsMessageEvent.lParam));

                browser->GetHost()->SendKeyEvent(keyEvent);

                break;
            }
        }
    }

    // util_win.cc
    bool CefMain::IsKeyDown(WPARAM wParam)
    {
        return (::GetKeyState(static_cast<int>(wParam)) & 0x8000) != 0;
    }

    // util_win.cc
    int CefMain::GetNativeCefKeyboardModifiers(WPARAM wParam, LPARAM lParam)
    {
        int modifiers = 0;

        if (IsKeyDown(VK_SHIFT))
        {
            modifiers |= EVENTFLAG_SHIFT_DOWN;
        }
        if (IsKeyDown(VK_CONTROL))
        {
            modifiers |= EVENTFLAG_CONTROL_DOWN;
        }
        if (IsKeyDown(VK_MENU))
        {
            modifiers |= EVENTFLAG_ALT_DOWN;
        }

        // Low bit set from GetKeyState indicates "toggled".
        if (::GetKeyState(VK_NUMLOCK) & 1)
        {
            modifiers |= EVENTFLAG_NUM_LOCK_ON;
        }
        if (::GetKeyState(VK_CAPITAL) & 1)
        {
            modifiers |= EVENTFLAG_CAPS_LOCK_ON;
        }

        switch (wParam)
        {
            case VK_RETURN:
            {
                if ((lParam >> 16) & KF_EXTENDED)
                {
                    modifiers |= EVENTFLAG_IS_KEY_PAD;
                }
                break;
            }

            case VK_INSERT:
            case VK_DELETE:
            case VK_HOME:
            case VK_END:
            case VK_PRIOR:
            case VK_NEXT:
            case VK_UP:
            case VK_DOWN:
            case VK_LEFT:
            case VK_RIGHT:
            {
                if (!((lParam >> 16) & KF_EXTENDED))
                {
                    modifiers |= EVENTFLAG_IS_KEY_PAD;
                }
                break;
            }

            case VK_NUMLOCK:
            case VK_NUMPAD0:
            case VK_NUMPAD1:
            case VK_NUMPAD2:
            case VK_NUMPAD3:
            case VK_NUMPAD4:
            case VK_NUMPAD5:
            case VK_NUMPAD6:
            case VK_NUMPAD7:
            case VK_NUMPAD8:
            case VK_NUMPAD9:
            case VK_DIVIDE:
            case VK_MULTIPLY:
            case VK_SUBTRACT:
            case VK_ADD:
            case VK_DECIMAL:
            case VK_CLEAR:
            {
                modifiers |= EVENTFLAG_IS_KEY_PAD;
                break;
            }

            case VK_SHIFT:
            {
                if (IsKeyDown(VK_LSHIFT))
                {
                    modifiers |= EVENTFLAG_IS_LEFT;
                }
                else if (IsKeyDown(VK_RSHIFT))
                {
                    modifiers |= EVENTFLAG_IS_RIGHT;
                }
                break;
            }

            case VK_CONTROL:
            {
                if (IsKeyDown(VK_LCONTROL))
                {
                    modifiers |= EVENTFLAG_IS_LEFT;
                }
                else if (IsKeyDown(VK_RCONTROL))
                {
                    modifiers |= EVENTFLAG_IS_RIGHT;
                }
                break;
            }

            case VK_MENU:
            {
                if (IsKeyDown(VK_LMENU))
                {
                    modifiers |= EVENTFLAG_IS_LEFT;
                }
                else if (IsKeyDown(VK_RMENU))
                {
                    modifiers |= EVENTFLAG_IS_RIGHT;
                }
                break;
            }

            case VK_LWIN:
            {
                modifiers |= EVENTFLAG_IS_LEFT;
                break;
            }

            case VK_RWIN:
            {
                modifiers |= EVENTFLAG_IS_RIGHT;
                break;
            }
        }

        return modifiers;
    }

    void CefMain::ToggleDevToolsWindow()
    {
        if (browser->GetHost()->HasDevTools())
        {
            browser->GetHost()->CloseDevTools();
        }
        else
        {
            SDL_SysWMinfo sysInfo;
            SDL_VERSION(&sysInfo.version);
            if (!SDL_GetWindowWMInfo(window, &sysInfo))
            {
                return;
            }

            CefBrowserSettings browserSettings;
            CefWindowInfo windowInfo;
            windowInfo.SetAsPopup(sysInfo.info.win.window, "DevTools");
            browser->GetHost()->ShowDevTools(windowInfo, uiClient, browserSettings, CefPoint(0, 0));
        }
    }
#endif
}
