#include "stdafx.h"

#include "CefBrowserClientFrameWnd.h"
#include "main/BrowserWndManager.h"

//#include "cef_simple/simple_app.h"
//#include "cef_simple/simple_handler.h"

#include "base/control/UICefBrowser.h"


//////////////////////////////////////////////////////////////////////////
CCefBrowserClientFrameWnd::CCefBrowserClientFrameWnd(const wstring& url)
	:CWndFrameBase()
{
	m_pCtaOnlyBrowserUI = new CCefBrowserUI(this, url);
}

CCefBrowserClientFrameWnd::~CCefBrowserClientFrameWnd(void)
{

}

void CCefBrowserClientFrameWnd::OnPrepare()
{

}

CDuiString CCefBrowserClientFrameWnd::GetSkinFile()
{
	return _T("frame_wnd_demo\\cef_browser_frame_client.xml");
}

void CCefBrowserClientFrameWnd::Notify(TNotifyUI& msg)
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
			if (m_pCtaOnlyBrowserUI && m_pCtaOnlyBrowserUI->CanGoBack())
				m_pCtaOnlyBrowserUI->GoBack();
		}
		else if (_tcscmp(pszCtrlName, _T("btn_forward")) == 0)
		{
			if (m_pCtaOnlyBrowserUI && m_pCtaOnlyBrowserUI->CanGoForward())
				m_pCtaOnlyBrowserUI->GoForward();
		}
		else if (_tcscmp(pszCtrlName, _T("btn_refresh")) == 0
			|| _tcscmp(pszCtrlName, _T("btn_refresh_new")) == 0)
		{
			if (m_pCtaOnlyBrowserUI)
				m_pCtaOnlyBrowserUI->Reload();
		}
		else if (_tcscmp(pszCtrlName, _T("btn_stop")) == 0)
		{
			if (m_pCtaOnlyBrowserUI)
				m_pCtaOnlyBrowserUI->StopLoad();
		}
		else if (_tcscmp(pszCtrlName, _T("btn_run_js")) == 0)
		{
			if (m_pCtaOnlyBrowserUI)
				m_pCtaOnlyBrowserUI->RunJs(_T("alert('ExecuteJavaScript works!  This is test');"));
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

			LoadUrl(strUrl);	
		}
	} 
	else if (_tcsicmp(msg.sType, _T("selectchanged")) == 0) {
		OnSelChanged(msg.pSender);
	}
}

void CCefBrowserClientFrameWnd::OnSelChanged(CControlUI* pSender)
{
	CTabLayoutUI* pTabTest = static_cast <CTabLayoutUI*>(m_pm.FindControl(_T("tabTest")));
	COptionUI *pCefControlTest = static_cast<COptionUI*>(m_pm.FindControl(_T("CEF_Control_Test")));
	if (pTabTest == NULL)
	{
		return;
	}
	CDuiString strSelName = pSender->GetName();
	if (strSelName == _T("CEF_Control_Test")) 
	{
		//if (m_pCtaOnlyBrowserUI) {
		//	m_pCtaOnlyBrowserUI->Navigate2(_T("http://192.168.1.251/main"));
		//}
		//ShowCefWindow();
		pTabTest->SelectItem(0);
	} else if (strSelName == _T("OptionDemo2")) {
		//HideCefWindow();
		pTabTest->SelectItem(1);
		TrayWindowManager::getInstance()->CreateRootWindowAsPopup(L"http://127.0.0.1:8080/cta_multi.html");
	}
}

CControlUI* CCefBrowserClientFrameWnd::CreateControl(LPCTSTR pstrClass, CPaintManagerUI *pManager)
{
	if( _tcsicmp(pstrClass, _T("Cef")) == 0){
		//m_pCtaOnlyBrowserUI = new CCefBrowserUI(this, _T(""));
		return m_pCtaOnlyBrowserUI;
	}

	return NULL;
}

LRESULT CCefBrowserClientFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	__super::OnSize(uMsg, wParam, lParam, bHandled);
	return 0;
}

void CCefBrowserClientFrameWnd::ShowCefWindow()
{	
	if (m_pCtaOnlyBrowserUI != NULL)
	{
		m_pCtaOnlyBrowserUI->SetVisible(true);
	}
}

void CCefBrowserClientFrameWnd::HideCefWindow()
{
	if (m_pCtaOnlyBrowserUI != NULL)
	{
		m_pCtaOnlyBrowserUI->SetVisible(false);
	}
}
void CCefBrowserClientFrameWnd::Minimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

void CCefBrowserClientFrameWnd::Maximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

void CCefBrowserClientFrameWnd::Restore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
}

void CCefBrowserClientFrameWnd::SetMinInfo(int cx, int cy)
{
	m_pm.SetMinInfo(cx, cy);
}

void CCefBrowserClientFrameWnd::LoadUrl(wstring url)
{
	if (m_pCtaOnlyBrowserUI){
		if (url.empty()) {
			std::wstring text = _T("address invalid");
			return;
		}
		
		m_pCtaOnlyBrowserUI->Navigate2(url);
	}
}

wstring CCefBrowserClientFrameWnd::GetUrl()
{
	if (m_pCtaOnlyBrowserUI){
		return m_pCtaOnlyBrowserUI->GetCurrentUrl();
	}
	return L"";
}

wstring CCefBrowserClientFrameWnd::GetTitle()
{
	if (m_title_label){
		return m_title_label->GetText().GetData();
	}
	return L"";
}

LRESULT CCefBrowserClientFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int zoom_index = 1;
	bool ctrl_state = false;

	switch(uMsg)
	{
	case WM_PRINT_WND_CLOSED:
		{
			::EnableWindow(this->GetHWND(), TRUE);
			//print_wnd_ = NULL;
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

void CCefBrowserClientFrameWnd::InitWindow()
{
	m_pOkBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_ok")));
	m_pAddressEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_address")));
	m_pBackBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_back")));
	m_pForwardBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_forward")));
	m_pRefreshBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_refresh")));
	m_pRefreshBtnNew = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_refresh_new")));
	m_pStopBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_stop")));
	m_title_label = static_cast<CLabelUI*>(m_pm.FindControl(L"title_label"));
}

LRESULT CCefBrowserClientFrameWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
void CCefBrowserClientFrameWnd::OnSetAddress(const std::wstring& url)
{
	if (m_pAddressEdit != nullptr){
		m_pAddressEdit->SetText(url.c_str());
	}
}

void CCefBrowserClientFrameWnd::OnSetTitle(const std::wstring& title) 
{
	if (m_title_label){
		m_title_label->SetText(title.c_str());
	}
	SetWindowText(m_hWnd, title.c_str());
}

void CCefBrowserClientFrameWnd::OnSetFullscreen(bool fullscreen) 
{
	if (fullscreen){
		Maximize();
	}else{
		Restore();
	}
}

void CCefBrowserClientFrameWnd::OnSetLoadingState(bool isLoading,
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
void CCefBrowserClientFrameWnd::OnSetDraggableRegions(
	const std::vector<CefDraggableRegion>& regions)
{

}

// Set load error message log
void CCefBrowserClientFrameWnd::OnLoadError(CefRefPtr<CefFrame> frame
								, int errorCode
								, const CefString& errorText
								, const CefString& failedUrl)
{

}

void CCefBrowserClientFrameWnd::OnMinimized()
{
	Minimize();
}

void CCefBrowserClientFrameWnd::OnExit()
{
	Close(IDOK);
}

void CCefBrowserClientFrameWnd::OnZoomValueChanged(int step)
{
	double scale = m_pCtaOnlyBrowserUI->GetZoomLevel(); 
	scale = scale + step * 0.2;
	m_pCtaOnlyBrowserUI->SetZoomLevel(scale);
}

void CCefBrowserClientFrameWnd::OnDoSpecialSomething(int type_id, std::wstring command)
{
	if (type_id == 1) {
		COptionUI* pControl = static_cast<COptionUI*>(m_pm.FindControl(_T("OptionDemo3")));
		if (pControl) {
			static int click_count = 1;
			CDuiString text;
			text.Format(_T("%s %2d"), command.c_str(), click_count);
			pControl->SetText(text);
			click_count++;
		}
	}
}
//////////////////////////////////////////////////////////////////////////

void CCefBrowserClientFrameWnd::NotifyWindowDestroyed()
{
	if (m_pDelegate)
		m_pDelegate->OnRootWindowDestroyed(this);
}

void CCefBrowserClientFrameWnd::SetBrowserZoomLevel(double level)
{
	if (m_pCtaOnlyBrowserUI) {
		m_pCtaOnlyBrowserUI->SetZoomLevel(level);
	}
}