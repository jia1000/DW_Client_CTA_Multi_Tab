/****************************************************************************************************

作者: jiayf 2018-12-14
备注: 托盘菜单界面
审核:

Copyright(c) 2018-2030 DeepWise Corporation

****************************************************************************************************/
#pragma once

class CWndFrameBase;

class MenuTray_Wnd : public DuiLib::CMenuWnd
{
public:
    MenuTray_Wnd();
	~MenuTray_Wnd();

    virtual bool ShowCustomMenu(const long x, const long y);
    void Notify(TNotifyUI& msg);    

    void OnFinalMessage( HWND hWnd);

private:
    std::wstring m_xml_filename;
    CWndFrameBase* m_main_wnd;
};
