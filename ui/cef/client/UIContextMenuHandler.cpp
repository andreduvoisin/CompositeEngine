#include "UIContextMenuHandler.h"

void UIContextMenuHandler::OnBeforeContextMenu(
        CefRefPtr<CefBrowser> /*browser*/,
        CefRefPtr<CefFrame> /*frame*/,
        CefRefPtr<CefContextMenuParams> /*params*/,
        CefRefPtr<CefMenuModel> model)
{
    // Disable the default right-click context menu.
    model->Clear();
}
