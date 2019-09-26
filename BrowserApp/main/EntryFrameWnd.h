#pragma once
#include <UIlib.h>
using namespace DuiLib;

//////////////////////////////////////////////////////////////////////////
// 该类作为学习Duilib各种控件的入口窗口
//////////////////////////////////////////////////////////////////////////

class MultiSlicesImageDemo;

class CEntryFrameWnd : public WindowImplBase
{
public:
	CEntryFrameWnd(void);
	virtual ~CEntryFrameWnd(void);

	virtual LPCTSTR GetWindowClassName() const ;
	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual CControlUI* CreateControl(LPCTSTR pstrClass, CPaintManagerUI *pManager);

	virtual void    Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	MultiSlicesImageDemo* box_widgets1;
	MultiSlicesImageDemo* box_widgets;
};

