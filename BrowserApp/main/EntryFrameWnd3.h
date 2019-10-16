#pragma once
#include <UIlib.h>
using namespace DuiLib;

//////////////////////////////////////////////////////////////////////////
// 该类作为学习Duilib各种控件的入口窗口
//////////////////////////////////////////////////////////////////////////

class MultiSlicesImageDemoSameSource;
class CDicomView;

class CEntryFrameWnd3 : public WindowImplBase
{
public:
	CEntryFrameWnd3(void);
	virtual ~CEntryFrameWnd3(void);

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
	MultiSlicesImageDemoSameSource* m_multiSliceImageDemo;

};

