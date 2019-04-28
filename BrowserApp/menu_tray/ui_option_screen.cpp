#include "StdAfx.h"
#include "ui_option_screen.h"

#include "main/BrowserWndManager.h"
#include "screen/screen_info_controller.h"

namespace DuiLib
{
    IMPLEMENT_DUICONTROL(COptionUIScreen)
        COptionUIScreen::COptionUIScreen() : COptionUI()
    {
    }

    COptionUIScreen::~COptionUIScreen()
    {
        if( !m_sGroupName.IsEmpty() && m_pManager ) m_pManager->RemoveOptionGroup(m_sGroupName, this);
    }

    LPCTSTR COptionUIScreen::GetClass() const
    {
        return _T("OptionScreenUI");
    }

    LPVOID COptionUIScreen::GetInterface(LPCTSTR pstrName)
    {
        if( _tcsicmp(pstrName, _T("OptionScreen")) == 0 ) return static_cast<COptionUIScreen*>(this);
        return CButtonUI::GetInterface(pstrName);
    }

    void COptionUIScreen::DoEvent(TEventUI& event)
    {
        if( event.Type == UIEVENT_BUTTONUP ) {
            
            if (IsNormalOfDisplay()) {
                std::wstring text = this->GetText();
                //if (std::wstring::npos == text.find(_T("(默认)"))) {
                //    text += _T("(默认)");
                //    SetText(text.c_str());
                //}
            } else {
                // 当前点击的屏幕，已经不存在了
                MessageBox(NULL, _T("所选屏幕已经不存在！"), _T("提示"), 0); 
                return;
            }
        }        
        COptionUI::DoEvent(event);
    }    

    bool COptionUIScreen::IsNormalOfDisplay()
    {
        std::map<std::wstring, std::wstring> screen_shots = ScreenInfoController::GetInstance()->GetScreenShot(); 

        if (screen_shots.size() <= 0) {
            return false;
        }

        std::wstring self_option_name(this->GetName());

        for (auto iter = screen_shots.begin(); iter !=  screen_shots.end(); ++iter) {            
            std::wstring option_name = std::wstring(_T("screen_option")) + std::wstring(_T("_")) + iter->first;
            if (self_option_name == option_name) {
                return true;
            }
        }    

        return false;
    }
}