#pragma once
#include <UIlib.h>
using namespace DuiLib;

//////////////////////////////////////////////////////////////////////////
// ������ΪѧϰDuilib���ֿؼ�����ڴ���
//////////////////////////////////////////////////////////////////////////

class MultiSlicesImageDemoSameSource;
class CDicomView;

class CEntryFrameWnd2 : public WindowImplBase
{
public:
	CEntryFrameWnd2(void);
	virtual ~CEntryFrameWnd2(void);

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
