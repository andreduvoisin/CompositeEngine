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
		CefMain(
            EventSystem* eventSystem,
            SDL_Window* g_window);

        bool StartCef(
            int argc,
            char* argv[],
            uint32_t SCREEN_WIDTH,
            uint32_t SCREEN_HEIGHT);
        void StopCef();

        // TODO: This probably shouldn't be architected this way.
        const char* GetViewBuffer();
        const char* GetPopupBuffer();
        const CefRect& GetPopupRect();

		// EventListener Interface
		void OnEvent(const Event& event) override;

	private:
	    void HandleSdlEvent(const Event& event);
        unsigned GetSdlCefInputModifiers(const SDL_Event& event);
#ifdef _WIN32
        void ToggleDevToolsWindow();
#endif

		EventSystem* eventSystem;
        SDL_Window* g_window;

        CefRefPtr<UIClient> g_uiClient;
        CefRefPtr<CefBrowser> g_browser;
        UIExternalMessagePump* externalMessagePump;
        UIQueryHandler* queryHandler;
	};
}

#endif //_CE_CEF_MAIN_H_