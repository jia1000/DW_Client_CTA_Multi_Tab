#include "stdafx.h"

#include "EntryFrameWnd.h"
#include "MultiSlicesImageDemo.h"
#include "vtk_dicom_demo/DicomView.h"

#include <thread>

CEntryFrameWnd::CEntryFrameWnd(void)
{
	dicom_viewxy = NULL;
	dicom_viewyz = NULL;
	dicom_viewxz = NULL;
	box_widgets  = NULL;
}

CEntryFrameWnd::~CEntryFrameWnd(void)
{
	if (dicom_viewxy) {
		delete dicom_viewxy;
		dicom_viewxy = NULL;
	}
	if (dicom_viewyz) {
		delete dicom_viewyz;
		dicom_viewyz = NULL;
	}
	if (dicom_viewxz) {
		delete dicom_viewxz;
		dicom_viewxz = NULL;
	}
	if (box_widgets) {
		delete box_widgets;
		box_widgets = NULL;
	}
	//PostQuitMessage(0);	
}

LPCTSTR CEntryFrameWnd::GetWindowClassName() const 
{ 
	return _T("CefBrowserFrameWnd"); 
}

void CEntryFrameWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this; 
}

void CEntryFrameWnd::InitWindow()
{
	//CButtonUI* pVtkShowBtn1 = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button3")));
	//if (pVtkShowBtn1) {
	//	pVtkShowBtn1->SetText(_T("Please wait..."));
	//}
	//box_widgets1 = new MultiSlicesImageDemo(this->m_hWnd, pVtkShowBtn1, false);
	//std::thread th1(&MultiSlicesImageDemo::ShowWidgets_Test, box_widgets1);
	//th1.detach();

	//CButtonUI* pVtkShowBtn2 = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button4")));
	//if (pVtkShowBtn2) {
	//	pVtkShowBtn2->SetText(_T("Please wait..."));
	//}
	//box_widgets = new MultiSlicesImageDemo(this->m_hWnd, pVtkShowBtn2, true);			
	//std::thread th2(&MultiSlicesImageDemo::ShowWidgets_Test, box_widgets);
	//th2.detach();
	//box_widgets->ShowWidgets_Test();
}

CControlUI* CEntryFrameWnd::CreateControl(LPCTSTR pstrClass, CPaintManagerUI *pManager)
{
	return NULL;
}

CDuiString CEntryFrameWnd::GetSkinFile()
{
	return _T("frame_wnd_demo\\entry_frame.xml");
}

CDuiString CEntryFrameWnd::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");

}

LRESULT CEntryFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//
	__super::OnSize(uMsg, wParam, lParam, bHandled);
	return 0;
}

void    CEntryFrameWnd::Notify(TNotifyUI& msg) 
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
			CButtonUI* pVtkShowBtn2 = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button4")));
			if (pVtkShowBtn2) {
				pVtkShowBtn2->SetText(_T("Please wait..."));
			}
			box_widgets = new MultiSlicesImageDemo(this->m_hWnd, pVtkShowBtn2, true);
			box_widgets->ShowWidgets_Test();
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
				if (item_name.CompareNoCase(L"function_3") == 0) {
					CButtonUI* pVtkShowBtn2 = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button1")));
					if (pVtkShowBtn2) {
						pVtkShowBtn2->SetText(_T("Please wait..."));					

						RECT rc = pVtkShowBtn2->GetClientPos();
						if (!dicom_viewxy) {
							dicom_viewxy = new CDicomView(this->m_hWnd, vtkImageViewer2::SLICE_ORIENTATION_XY, rc);
							dicom_viewxy->InitVtk("..\\bin\\release\\Skin\\data\\slices1");
						}
						dicom_viewxy->SetOrientation(vtkImageViewer2::SLICE_ORIENTATION_XY);
					}					
				} else if (item_name.CompareNoCase(L"function_4") == 0) {
					CButtonUI* pVtkShowBtn2 = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button2")));
					if (pVtkShowBtn2) {
						pVtkShowBtn2->SetText(_T("Please wait..."));					

						RECT rc = pVtkShowBtn2->GetClientPos();
						if (!dicom_viewyz) {
							dicom_viewyz = new CDicomView(this->m_hWnd, vtkImageViewer2::SLICE_ORIENTATION_YZ, rc);
							dicom_viewyz->InitVtk("..\\bin\\release\\Skin\\data\\slices1");
						}
						dicom_viewyz->SetOrientation(vtkImageViewer2::SLICE_ORIENTATION_YZ);
					}					
				} else if (item_name.CompareNoCase(L"function_5") == 0) {
					CButtonUI* pVtkShowBtn2 = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button3")));
					if (pVtkShowBtn2) {
						pVtkShowBtn2->SetText(_T("Please wait..."));					

						RECT rc = pVtkShowBtn2->GetClientPos();
						if (!dicom_viewxz) {
							dicom_viewxz = new CDicomView(this->m_hWnd, vtkImageViewer2::SLICE_ORIENTATION_XZ, rc);
							dicom_viewxz->InitVtk("..\\bin\\release\\Skin\\data\\slices1");
						}
						dicom_viewxz->SetOrientation(vtkImageViewer2::SLICE_ORIENTATION_XZ);
					}
				} else if (item_name.CompareNoCase(L"function_3d_01") == 0) {

				}else if (item_name.CompareNoCase(L"function_3d_move_sagitta") == 0) {
					if (box_widgets) {
						box_widgets->ShowWidgets_Move_Sagitta();
					}
				}else if (item_name.CompareNoCase(L"function_3d_move_cornal") == 0) {
					if (box_widgets) {
						box_widgets->ShowWidgets_Move_Cornal();
					}
				}else if (item_name.CompareNoCase(L"function_3d_move_axial") == 0) {
					if (box_widgets) {
						box_widgets->ShowWidgets_Move_Axial();
					}
				}else if (item_name.CompareNoCase(L"function_3d_hue_coronal") == 0) {
					if (box_widgets) {
						box_widgets->ShowWidgets_Hue_coronal();
					}
				}else if (item_name.CompareNoCase(L"function_3d_show_hide_sagitta") == 0) {
					if (box_widgets) {
						box_widgets->ShowWidgets_show_Hide_Sagitta();
					}
				}else if (item_name.CompareNoCase(L"function_3d_show_hide_coronal") == 0) {
					if (box_widgets) {
						box_widgets->ShowWidgets_show_Hide_coronal();
					}
				}else if (item_name.CompareNoCase(L"function_3d_show_hide_axial") == 0) {
					if (box_widgets) {
						box_widgets->ShowWidgets_show_Hide_Axial();
					}
				}else if (item_name.CompareNoCase(L"function_3d_show_hide_bone") == 0) {
					if (box_widgets) {
						box_widgets->ShowWidgets_show_Hide_bone();
					}
				}else if (item_name.CompareNoCase(L"function_3d_show_hide_skin") == 0) {
					if (box_widgets) {
						box_widgets->ShowWidgets_show_Hide_skin();
					}
				}else if (item_name.CompareNoCase(L"function_3d_show_hide_outline") == 0) {
					if (box_widgets) {
						box_widgets->ShowWidgets_show_Hide_outline();
					}
				}else if (item_name.CompareNoCase(L"function_3d_mip_coronal") == 0) {
					if (box_widgets) {
						box_widgets->ShowWidgets_Mip_Cornal();
					}
				}
				// 其它二级菜单
			}
		}
	}
}

LRESULT CEntryFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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