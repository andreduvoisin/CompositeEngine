#ifndef _CE_CEF_MAIN_H_
#define _CE_CEF_MAIN_H_

#include "event/core/EventListener.h"
#include "include/cef_base.h"

class EventSystem;

class UIClient;
class CefBrowser;
class UIExternalMessagePump;
class UIQueryHandler;

union SDL_Event;
struct SDL_Window;

namespace CE
{
    class CefMain : public EventListener
    {
    public:
        CefMain(EventSystem* eventSystem, SDL_Window* window);

        // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays, modernize-avoid-c-arrays)
        bool StartCef(int argc, char* argv[], uint32_t screenWidth, uint32_t screenHeight);
        void StopCef();

        // TODO: This probably shouldn't be architected this way.
        const std::byte* GetViewBuffer();
        const std::byte* GetPopupBuffer();
        const CefRect& GetPopupRect();
        bool HasPopup();

        // EventListener Interface
        void OnEvent(const Event& event) override;

    private:
        void HandleSdlEvent(const Event& event);
        unsigned GetSdlCefInputModifiers(const SDL_Event& event);

#ifdef _WIN32
        void HandleWindowsMessageEvent(const Event& event);
        bool IsKeyDown(WPARAM wParam);
        int GetNativeCefKeyboardModifiers(WPARAM wParam, LPARAM lParam);
        void ToggleDevToolsWindow();
#endif

        EventSystem* eventSystem;
        SDL_Window* window;

        CefRefPtr<UIClient> uiClient;
        CefRefPtr<CefBrowser> browser;
        UIExternalMessagePump* externalMessagePump;
        UIQueryHandler* queryHandler;
    };
}

#endif //_CE_CEF_MAIN_H_