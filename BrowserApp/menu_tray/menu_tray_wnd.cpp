#include "stdafx.h"
#include "menu_tray_wnd.h"

#include "main/BrowserWndManager.h"
#include "menu_tray/menu_tray_set_wnd.h"

MenuTray_Wnd::MenuTray_Wnd() : m_xml_filename(L"menu_tray/menu_tray.xml")
{
}

MenuTray_Wnd::~MenuTray_Wnd()
{
}

void MenuTray_Wnd::OnFinalMessage( HWND hWnd)
{
    __super::OnFinalMessage(hWnd);
    delete this;
}

bool MenuTray_Wnd::ShowCustomMenu(const long x, const long y)
{
    CDuiPoint point;
    ::GetCursorPos(&point);

    // 暂时鼠标点固定移动
    POINT pt_wnd = {x -120, y - 64};
    ::ClientToScreen(GetHWND(), &pt_wnd);

    Init(NULL, m_xml_filename.c_str(), pt_wnd, &m_pm);
    return false;
}

void MenuTray_Wnd::Notify(TNotifyUI& msg)
{
    if (_tcsicmp(msg.sType, _T("click")) == 0) {
        if( msg.pSender->GetName() == _T("tray_menu_set") ) {
            // 打开设置窗口
            TrayWindowManager::getInstance()->CreateMenuTraySetWnd();

        }else if( msg.pSender->GetName() == _T("tray_menu_close") ) {
            TrayWindowManager::getInstance()->DestroyedTrayManager();
        }
    }
}
