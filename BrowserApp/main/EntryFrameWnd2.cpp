#include "stdafx.h"

#include "EntryFrameWnd2.h"
#include "vtk_dicom_demo/DicomView.h"
#include "MultiSlicesImageDemoSameSource.h"

#include <thread>

CEntryFrameWnd2::CEntryFrameWnd2(void)
{
	
}

CEntryFrameWnd2::~CEntryFrameWnd2(void)
{
	
	//PostQuitMessage(0);	
}

LPCTSTR CEntryFrameWnd2::GetWindowClassName() const 
{ 
	return _T("CefBrowserFrameWnd"); 
}

void CEntryFrameWnd2::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this; 
}

void CEntryFrameWnd2::InitWindow()
{
	
}

CControlUI* CEntryFrameWnd2::CreateControl(LPCTSTR pstrClass, CPaintManagerUI *pManager)
{
	return NULL;
}

CDuiString CEntryFrameWnd2::GetSkinFile()
{
	return _T("frame_wnd_demo\\entry_frame2.xml");
}

CDuiString CEntryFrameWnd2::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");

}

LRESULT CEntryFrameWnd2::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//
	__super::OnSize(uMsg, wParam, lParam, bHandled);
	return 0;
}

void    CEntryFrameWnd2::Notify(TNotifyUI& msg) 
{
	if (msg.sType == _T("click")) {
		LPCTSTR pszCtrlName = msg.pSender->GetName().GetData();
		if (_tcscmp(pszCtrlName, _T("btn_sysmin")) == 0) {
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		} else if (_tcscmp(pszCtrlName, _T("btn_sysmax")) == 0) {
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			// 最大化和还原2个button之间的切换
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_sysmax")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_restore")));
			if( pControl ) pControl->SetVisible(true);
		} else if (_tcscmp(pszCtrlName, _T("btn_restore")) == 0) {
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
			// 最大化和还原2个button之间的切换
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_sysmax")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_restore")));
			if( pControl ) pControl->SetVisible(false);
		} else if (_tcscmp(pszCtrlName, _T("btn_sysclose")) == 0) {
			Close(IDOK);
		} else if (_tcscmp(pszCtrlName, _T("Button1")) == 0) {
			
		} else if (_tcscmp(pszCtrlName, _T("Button2")) == 0) {
			
		} else if (_tcscmp(pszCtrlName, _T("Button3")) == 0) {
						
		} else if (_tcscmp(pszCtrlName, _T("Button4")) == 0) {
		} 
	} else if (_tcsicmp(msg.sType, _T("selectchanged")) == 0) {
		//
	} else if (msg.sType == _T("itemclick")){        
		// 改双击收缩为单击收缩
		CTreeNodeUI * pTreeNode = static_cast<CTreeNodeUI*>(msg.pSender);
		CDuiString item_name = msg.pSender->GetName();
		if (pTreeNode && _tcsicmp(pTreeNode->GetClass(), _T("TreeNodeUI")) == 0) {
			if (pTreeNode->IsHasChild()) {
				// 如果是一级菜单
				CTreeViewUI	* pTreeView = pTreeNode->GetTreeView();
				if (pTreeView) {
					CCheckBoxUI* pFolder = pTreeNode->GetFolderButton();
					pFolder->Selected(!pFolder->IsSelected());
					pTreeNode->SetVisibleTag(!pFolder->GetCheck());
					pTreeView->SetItemExpand(!pFolder->GetCheck(), pTreeNode);
				}
				// 点击第一级菜单，会选择第二级菜单的第一个
				if (item_name.CompareNoCase(L"vtk_function_test") == 0) {
					CButtonUI* pControl = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button1")));
					if (pControl) {
						pControl->SetText(_T("Group click."));
					}
				}
			} else {
				// 如果是二级菜单
				RECT rc;
				CButtonUI* pVtkShowBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button_vtk_function_show")));
				if (pVtkShowBtn) {
					rc = pVtkShowBtn->GetClientPos();
				}
				CButtonUI* pControl = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button_vtk_function_show")));
				if (item_name.CompareNoCase(L"function_1") == 0) {
					CButtonUI* pVtkShowBtn2 = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button1")));
					if (pVtkShowBtn2) {
						pVtkShowBtn2->SetText(_T("Please wait..."));
					}
					m_multiSliceImageDemo = new MultiSlicesImageDemoSameSource(this->m_hWnd, pVtkShowBtn2, true);
					m_multiSliceImageDemo->ShowWidgets_Test();
				} else if (item_name.CompareNoCase(L"function_3d_move_sagitta") == 0) {					
				}
				// 其它二级菜单
			}
		}
	}
}

LRESULT CEntryFrameWnd2::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;

	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_SIZE:
		OnSize(uMsg, wParam, lParam, bHandled);
		break;
	}

	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) {
		return lRes;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}