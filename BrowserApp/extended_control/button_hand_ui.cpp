#include "stdafx.h"
#include "button_hand_ui.h"

CButtonHandUI::CButtonHandUI()
{
}

CButtonHandUI::~CButtonHandUI(void)
{

}

LPCTSTR CButtonHandUI::GetClass() const
{
    return _T("ButtonHandUI");
}

LPVOID CButtonHandUI::GetInterface( LPCTSTR pstrName )
{
    if( _tcscmp(pstrName, _T("ButtonHand")) == 0 ) 
    {
        return static_cast<CButtonHandUI*>(this);
    }
    return CButtonUI::GetInterface(pstrName);
}

void CButtonHandUI::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_MOUSEMOVE)
	{
		// 添加鼠标滑过，变为小手图标
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
			if( ::PtInRect(&m_rcItem, event.ptMouse) ) { 
				m_uButtonState |= UISTATE_PUSHED;
			}
			else {
				m_uButtonState &= ~UISTATE_PUSHED;
			}
			Invalidate();
		}
		return;
	}	
	CButtonUI::DoEvent(event);
}
