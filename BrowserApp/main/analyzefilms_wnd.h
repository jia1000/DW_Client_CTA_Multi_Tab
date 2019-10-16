#pragma once
#include <UIlib.h>
using namespace DuiLib;

//////////////////////////////////////////////////////////////////////////
// 该类作为学习Duilib各种控件的入口窗口
//////////////////////////////////////////////////////////////////////////

class MultiSlicesImageDemoSameSource;
class CDicomView;

class AnalyzeFilms_Wnd : public WindowImplBase
{
public:
	AnalyzeFilms_Wnd(void);
	virtual ~AnalyzeFilms_Wnd(void);

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

