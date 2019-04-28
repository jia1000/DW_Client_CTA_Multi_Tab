#ifndef MENU_TRAY_SET_WND_H_
#define MENU_TRAY_SET_WND_H_

#pragma once
using namespace DuiLib;
#include "Utils/WinImplBase.h"

enum ENUM_TREE_NODE_SELECTED{
    TREE_NODE_SCREEN_SET,
    TREE_NODE_SCREEN_FIXED,
    TREE_NODE_SCREEN_ZOOM,
    TREE_NODE_SCREEN_ABOUT,

    TREE_NODE_SELECTED_COUNT
};
class MenuTraySet_Wnd : public WindowImplBase
{
public:
    MenuTraySet_Wnd(void);
    ~MenuTraySet_Wnd(void);

public:
    virtual CDuiString GetSkinFile();
    virtual LPCTSTR GetWindowClassName(void) const;
    virtual void OnFinalMessage( HWND );
    virtual void Notify(TNotifyUI &msg);
    virtual void InitWindow();
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    virtual CControlUI* CreateControl(LPCTSTR pstrClass, CPaintManagerUI *pManager);

    std::wstring GetChromeCoreVersionText();

    // 设置Slider的index
    void SetSliderIndex(int index);         
    // 设置当前缩放倍数
    void SetZoomButtonValue(double value);

    DUI_DECLARE_MESSAGE_MAP()
    virtual void OnClick(TNotifyUI& msg);

private:
    // 更新用于显示“屏幕列表”的list控件
    void RefreshScreenListUI(bool is_force);

    // 更新每个“屏幕”的显示文本
    void UpdateScreensShowText(std::wstring cur_click_screen);

    // 屏幕选择后， 写入配置文件
    void SetSelectedScreenIndex(size_t shot_index);

    // Tab页签的切换
    void TabSwitchSelected(ENUM_TREE_NODE_SELECTED select_index);

    // 得到屏幕对应的控件名称
    std::wstring GetControlNameText(std::wstring display_name);

    //得到屏幕在控件上，显示的文本
    std::wstring GetShowText(int index);

    // 判断屏幕是否可以正常显示，如，断开电源时，不能显示
    bool IsNormalOfDisplay(std::wstring control_name, std::wstring& display_name);
    CListUI* m_pTagsListUI;
    std::map<std::wstring, std::wstring> m_screen_shots;
};

#endif //MENU_TRAY_SET_WND_H_