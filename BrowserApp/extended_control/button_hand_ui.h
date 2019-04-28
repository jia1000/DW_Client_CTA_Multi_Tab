#ifndef __CBUTTON_HAND_UI_H__
#define __CBUTTON_HAND_UI_H__

#pragma once
using namespace std;




class CButtonHandUI : public CButtonUI
{
	DECLARE_DUICONTROL(CButtonHandUI)
public:
	CButtonHandUI();
    ~CButtonHandUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    
	void DoEvent(TEventUI& event);
private:
};



#endif // __CBUTTON_HAND_UI_H__