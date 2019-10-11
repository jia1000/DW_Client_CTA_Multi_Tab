#include "stdafx.h"

#include "WndSimpleFrame.h"
#include "base/control/UICefBrowser.h"
#include "main/BrowserWndManager.h"
#include <regex>
#include "print_preview.h"

//const wchar_t kHotKeyZoomAdd[] = L"Dr_wise_client_zoom_add";
//ATOM g_hot_key_zoom_add = GlobalAddAtom(kHotKeyZoomAdd);
//const wchar_t kHotKeyZoomMinus[] = L"Dr_wise_client_zoom_minus";
//ATOM g_hot_key_zoom_minus = GlobalAddAtom(kHotKeyZoomMinus);

//DUI_BEGIN_MESSAGE_MAP(CWndFrameBase, WindowImplBase)
//	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
//DUI_END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
CWndFrameBase::CWndFrameBase(void)
{
	
}

CWndFrameBase::~CWndFrameBase(void)
{
	
}

void CWndFrameBase::OnPrepare()
{

}

void CWndFrameBase::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("windowinit"))
		OnPrepare();
	else if (msg.sType == _T("click"))
	{
		LPCTSTR pszCtrlName = msg.pSender->GetName().GetData();
		if (_tcscmp(pszCtrlName, _T("btn_sysmin")) == 0)
		{
			Minimize();
		}
        if (_tcscmp(pszCtrlName, _T("btn_sysmax")) == 0)
        {
            Maximize();
            CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_sysmax")));
            if( pControl ) pControl->SetVisible(false);
            pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_restore")));
            if( pControl ) pControl->SetVisible(true);
        }
        if (_tcscmp(pszCtrlName, _T("btn_restore")) == 0)
        {
            Restore();
            CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_sysmax")));
            if( pControl ) pControl->SetVisible(true);
            pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_restore")));
            if( pControl ) pControl->SetVisible(false);
        }
		else if (_tcscmp(pszCtrlName, _T("btn_sysclose")) == 0)
		{
            Close(IDOK);
		}
		else if (_tcscmp(pszCtrlName, _T("btn_ok")) == 0)
		{

		}
		else if (_tcscmp(pszCtrlName, _T("btn_back")) == 0)
		{
			if (m_pBrowserUI && m_pBrowserUI->CanGoBack())
				m_pBrowserUI->GoBack();
		}
		else if (_tcscmp(pszCtrlName, _T("btn_forward")) == 0)
		{
			if (m_pBrowserUI && m_pBrowserUI->CanGoForward())
				m_pBrowserUI->GoForward();
		}
		else if (_tcscmp(pszCtrlName, _T("btn_refresh")) == 0
            || _tcscmp(pszCtrlName, _T("btn_refresh_new")) == 0)
		{
			if (m_pBrowserUI)
				m_pBrowserUI->Reload();
		}
		else if (_tcscmp(pszCtrlName, _T("btn_dev_tools")) == 0)
		{
			CefWindowInfo windowInfo;
			windowInfo.SetAsPopup(NULL, L"cef_devtools");
			CefBrowserSettings settings;
			windowInfo.width = 900;
			windowInfo.height = 700;
			m_pBrowserUI2->m_pBrowser->GetHost()->ShowDevTools(windowInfo, m_pBrowserUI2->m_pClientHandler, settings, CefPoint());
		}
		else if (_tcscmp(pszCtrlName, _T("btn_stop")) == 0)
		{			
			if (m_pBrowserUI)
				m_pBrowserUI->StopLoad();
		}
	}
	else if (msg.sType == _T("return"))
	{
		LPCTSTR pszCtrlName = msg.pSender->GetName().GetData();
		if (_tcscmp(pszCtrlName, _T("edit_address")) == 0)
		{
			wstring strUrl = msg.pSender->GetText().GetData();
			if (strUrl.length() == 0){
				return;
			}

			if (strUrl.length() < 5){
				strUrl = L"http://" + strUrl;
			}
			else{
				wstring start = strUrl.substr(0,5);
				transform(start.begin(), start.end(), start.begin(), toupper);

				if (start != L"HTTP:" && start != L"HTTPS"){
					strUrl = L"http://" + strUrl;
				}
			}

			CGLogger::Debug("load url: " + WStringToString(strUrl));
			LoadUrl(strUrl);	
		}
	}
}

CControlUI* CWndFrameBase::CreateControl(LPCTSTR pstrClass, CPaintManagerUI *pManager)
{
	if( _tcsicmp(pstrClass, _T("Cef")) == 0){
		return m_pBrowserUI;
	}
	else if( _tcsicmp(pstrClass, _T("Cef2")) == 0){
		return m_pBrowserUI2;
	}
	return NULL;
}

void CWndFrameBase::Minimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

void CWndFrameBase::Maximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

void CWndFrameBase::Restore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
}

void CWndFrameBase::SetMinInfo(int cx, int cy)
{
	m_pm.SetMinInfo(cx, cy);
}

void CWndFrameBase::LoadUrl(wstring url)
{
	if (m_pBrowserUI){
		if (url.empty()) {
			std::wstring text = _T("address invalid");
			return;
		}
		// 关闭上一次打开的打印窗口
		ClosePrintWindow();

		m_pBrowserUI->Navigate2(url);
	}
}

wstring CWndFrameBase::GetUrl()
{
	if (m_pBrowserUI){
		return m_pBrowserUI->GetCurrentUrl();
	}
	return L"";
}

wstring CWndFrameBase::GetTitle()
{
	if (m_title_label){
		return m_title_label->GetText().GetData();
	}
	return L"";
}

LRESULT CWndFrameBase::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static int zoom_index = 1;
    bool ctrl_state = false;

	switch(uMsg)
	{
	case WM_PRINT_WND_CLOSED:
        {
            ::EnableWindow(this->GetHWND(), TRUE);
            print_wnd_ = NULL;
            break;
        }
    //case WM_HOTKEY:
    //    {
    //        zoom_index = Configurations::getInstance()->GetBrowserZoomLevelIndex();
    //        if (wParam== g_hot_key_zoom_add){
    //            zoom_index++;   
    //        } else if (wParam == g_hot_key_zoom_minus) {
    //            zoom_index--;
    //        }
    //        
    //        if (m_pDelegate) {
    //            TrayWindowManager* tray_manager = static_cast<TrayWindowManager*>(m_pDelegate);
    //            if (tray_manager) {                    
    //                tray_manager->NotifyZoomIndexChanged(zoom_index);
    //            }
    //        }
    //        break;
    //    }
	case WM_KEYDOWN:
		{
			bool ctrl_state = GetKeyState(VK_CONTROL) < 0;
			if (ctrl_state) {
				if (wParam == VK_ADD			// 数字键盘的 '+ '
					|| wParam == VK_OEM_PLUS	// '-" 
					) {
					OnZoomValueChanged(1);
				} if (wParam == VK_SUBTRACT		// 数字键盘的 '-'
					|| wParam == VK_OEM_MINUS	// '='
					) {
					OnZoomValueChanged(-1);
				}
			} 
			break;
		}
	default:
		break;
	}
	return __super::HandleMessage(uMsg,wParam,lParam);
}

void CWndFrameBase::InitWindow()
{
	m_pOkBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_ok")));
	m_pAddressEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_address")));
	m_pBackBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_back")));
	m_pForwardBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_forward")));
	m_pRefreshBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_refresh")));
	m_pRefreshBtnNew = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_refresh_new")));
	m_pStopBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_stop")));
 	m_title_label = static_cast<CLabelUI*>(m_pm.FindControl(L"title_label"));

    //RegisterHotKey(this->m_hWnd, g_hot_key_zoom_add,   MOD_CONTROL, VK_ADD);
    //RegisterHotKey(this->m_hWnd, g_hot_key_zoom_minus, MOD_CONTROL, VK_SUBTRACT );
}

LRESULT CWndFrameBase::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if defined(WIN32)
    BOOL bIconic = ::IsIconic(m_hWnd);
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(m_hWnd) != bZoomed){

		bool isMaximized = !bZoomed;

		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_sysmax")));
		if( pControl ) pControl->SetVisible(!isMaximized);
		pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_restore")));
		if( pControl ) pControl->SetVisible(isMaximized);
	}
    // 双屏且分辨率不同，从任务栏恢复窗口时最大化尺寸不对（实际是小屏幕的分辨率）
    // 原因：WinImplBase里响应两次WM_GETMINMAXINFO消息，第二次MonitorFromWindow获取不到屏幕，使用了默认的主屏幕尺寸造成
    // 应急解决方法：此情形下如果窗口尺寸不对，则对窗口先Restore再Maximize，强制最大化窗口到正确尺寸
    if (bIconic && (::IsIconic(m_hWnd) != bIconic)){
        // 从最小化处还原，且当前已经是最大化
        if (::IsZoomed(m_hWnd)){
            SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
            SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
        }
    } 
#else
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
#endif

	return 0;
}


//////////////////////////////////////////////////////////////////////////
// ICallbackDelegate events
void CWndFrameBase::OnSetAddress(const std::wstring& url)
{
	if (m_pAddressEdit != nullptr){
		m_pAddressEdit->SetText(url.c_str());
	}
}

void CWndFrameBase::OnSetTitle(const std::wstring& title) 
{
	if (m_title_label){
		m_title_label->SetText(title.c_str());
	}
	SetWindowText(m_hWnd, title.c_str());
}

void CWndFrameBase::OnSetFullscreen(bool fullscreen) 
{
	if (fullscreen){
		Maximize();
	}else{
		Restore();
	}
}

void CWndFrameBase::OnSetLoadingState(bool isLoading,
									  bool canGoBack,
									  bool canGoForward)
{
	if (m_pBackBtn){
		m_pBackBtn->SetEnabled(canGoBack);
	}
	if (m_pForwardBtn){
		m_pForwardBtn->SetEnabled(canGoForward);
	}
	if (m_pRefreshBtn){
		m_pRefreshBtn->SetEnabled(!isLoading);
	}
    if (m_pRefreshBtnNew){
        m_pRefreshBtnNew->SetEnabled(!isLoading);
    }
	if (m_pStopBtn){
		m_pStopBtn->SetEnabled(isLoading);
	}
}


//////////////////////////////////////////////////////////////////////////
// ICallbackDelegate methods
void CWndFrameBase::OnSetDraggableRegions(
	const std::vector<CefDraggableRegion>& regions)
{
	
}

// Set load error message log
void CWndFrameBase::OnLoadError(CefRefPtr<CefFrame> frame
				 , int errorCode
				 , const CefString& errorText
				 , const CefString& failedUrl)
{

}

void CWndFrameBase::OnMinimized()
{
	Minimize();
}

void CWndFrameBase::OnExit()
{
	Close(IDOK);
}

void CWndFrameBase::OnZoomValueChanged(int step)
{
	int zoom_index = Configurations::getInstance()->GetBrowserZoomLevelIndex();
	zoom_index += step;

	if (m_pDelegate) {
		TrayWindowManager* tray_manager = static_cast<TrayWindowManager*>(m_pDelegate);
		if (tray_manager) {                    
			tray_manager->NotifyZoomIndexChanged(zoom_index);
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void CWndFrameBase::NotifyWindowDestroyed()
{
	if (m_pDelegate)
		m_pDelegate->OnRootWindowDestroyed(this);
}

void CWndFrameBase::OpenPrintWindow(const wstring& url)
{
	// 计算窗体尺寸
	RECT rect;
	GetClientRect(this->GetHWND(), &rect); 
	int cxClient = (int)((rect.right - rect.left) * 0.90);  // 获得客户区宽度
	int cyClient = (int)((rect.bottom - rect.top) * 0.90);  // 获得客户区高度
	// 关闭上一次打印窗口
	ClosePrintWindow();
	// 创建打印预览窗体
	CWndPrint* childWnd = new CWndPrint(url);
	print_wnd_ = childWnd;
	childWnd->Create(this->GetHWND(), _T("PrintWnd")
		, WS_POPUPWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS
		, WS_EX_OVERLAPPEDWINDOW
		, 0, 0, 0, 0);

	childWnd->ResizeClient(cxClient, cyClient);
	childWnd->SetFromUrl(this->GetUrl());
	childWnd->SetFromTitle(this->GetTitle());
	// 先弹出窗口 [12/30/2018 Modified by zhangjian]
	childWnd->CenterWindow();
	HWND hWndParent = GetWindowOwner(m_hWnd);
	HWND tet = this->GetHWND();
	childWnd->ShowWindow();
	// 禁用浏览器窗口
	::EnableWindow(this->GetHWND(), FALSE);
	// end
	childWnd->StartPrintPreview();
}

void CWndFrameBase::ClosePrintWindow()
{
	if (print_wnd_){
		// 禁用浏览器窗口
		::EnableWindow(this->GetHWND(), TRUE);
		print_wnd_->Close();
		print_wnd_ = NULL;
	}
}

void CWndFrameBase::SetBrowserZoomLevel(double level)
{
    if (m_pBrowserUI) {
        m_pBrowserUI->SetZoomLevel(level);
    }
}