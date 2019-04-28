#include "StdAfx.h"

#include "print_preview.h"
// Use for enum printers
#include <Winspool.h>
#pragma comment(lib, "winspool.lib")
#include "main/BrowserWndManager.h"
#include "util/Path.h"

const int kPaddingMaximum = 199;
const int kA4Width = 210;
const int kA4Height = 297;

CWndPrint::CWndPrint(const wstring& url)
{
	m_pBrowserUI = new CCefBrowserUI(this, _CEF_BLANK_);
	m_sBrowserID = KEY_PRINTWINDOW;
	html_url_ = url;
	identity_url_ = L"http://print";
	window_loaded = false;

	ReadParameter();
}

CWndPrint::~CWndPrint()
{

}

void CWndPrint::OnFinalMessage(HWND hWnd)
{
	SaveParameter();

	WindowImplBase::OnFinalMessage(hWnd);

	if (pdf_creator_){
		delete pdf_creator_;
		pdf_creator_ = NULL;
	}
	if (last_pdf_file_.length() > 0){
		RemovePdfFile(last_pdf_file_);
	}
	if (display_pdf_file_.length() > 0){
		RemovePdfFile(display_pdf_file_);
	}

	delete this;
}

void CWndPrint::ReadParameter()
{
	background_enabled_ = false;
	landscape = false;

	header_footer_enabled_ = Configurations::getInstance()->GetHeaderEnabled();
	int margin_type = Configurations::getInstance()->GetMarginType();
	if (margin_type < 0 || margin_type > 3){
		margin_type = 0;
	}
	margin_type_ = (cef_pdf_print_margin_type_t)margin_type;
	Configurations::getInstance()->GetMargin(margin_left_, margin_right_, margin_top_, margin_bottom_);
	if (margin_left_ < 0 || margin_left_ > kPaddingMaximum){
		margin_left_ = 0;
	}
	if (margin_right_ < 0 || margin_right_ > kPaddingMaximum){
		margin_right_ = 0;
	}
	if (margin_top_ < 0 || margin_top_ > kPaddingMaximum){
		margin_top_ = 0;
	}
	if (margin_bottom_ < 0 || margin_bottom_ > kPaddingMaximum){
		margin_bottom_ = 0;
	}

	//Paper* p = new Paper(L"A4", 210, 297, true);
	//paper_list_.push_back(p);
	//Paper* p2 = new Paper(L"A5", 148, 210, true);
	//paper_list_.push_back(p2);
}

void CWndPrint::SaveParameter()
{
	Configurations::getInstance()->SetMarginType(margin_type_);
	Configurations::getInstance()->SetMargin(margin_left_, margin_right_, margin_top_, margin_bottom_);
	Configurations::getInstance()->SetHeaderEnabled(header_footer_enabled_);
}

void CWndPrint::SetFromUrl(const wstring& from_url)
{
	if (from_url.length() > 0)
	identity_url_ = from_url;
}

void CWndPrint::SetFromTitle(const wstring& title)
{
	identity_title_ = title;
}

void CWndPrint::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("windowinit"))
		OnPrepare();
	else if (msg.sType == _T("click"))
	{
		LPCTSTR pszCtrlName = msg.pSender->GetName().GetData();
		
		if (_tcscmp(pszCtrlName, _T("btn_print")) == 0)
		{
			if (m_pBrowserUI){
				// 调用browser窗口的打印接口，弹出系统打印对话框
				m_pBrowserUI->Print();
			}
		}
		else if (_tcscmp(pszCtrlName, _T("btn_cancel")) == 0)
		{
			HWND hWndParent = GetWindowOwner(m_hWnd);
			::EnableWindow(hWndParent, TRUE);
			// 通知父窗口
			::SendMessage(hWndParent, WM_PRINT_WND_CLOSED, 0, 0);
			Close(IDOK);
		}
		else if (_tcscmp(pszCtrlName, _T("option_header_footer")) == 0)
		{
			header_footer_enabled_ = !opt_header_footer_->IsSelected();

			PostMessage(WM_PRINT_PREVIEW);
		}
	}
	else if (msg.sType == _T("return")){
		// 响应页边距数值更改事件
		LPCTSTR pszCtrlName = msg.pSender->GetName().GetData();

		if (_tcscmp(pszCtrlName, _T("edit_left")) == 0)
		{
			margin_left_ = _wtoi(edit_padding_left_->GetText().GetData());
			PostMessage(WM_PRINT_PREVIEW);
		}
		else if (_tcscmp(pszCtrlName, _T("edit_right")) == 0)
		{
			margin_right_ = _wtoi(edit_padding_right_->GetText().GetData());
			PostMessage(WM_PRINT_PREVIEW);
		}
		else if (_tcscmp(pszCtrlName, _T("edit_top")) == 0)
		{
			margin_top_ = _wtoi(edit_padding_top_->GetText().GetData());
			PostMessage(WM_PRINT_PREVIEW);
		}
		else if (_tcscmp(pszCtrlName, _T("edit_bottom")) == 0)
		{
			margin_bottom_ = _wtoi(edit_padding_bottom_->GetText().GetData());
			PostMessage(WM_PRINT_PREVIEW);
		}
	}
	else if (msg.sType == _T("killfocus")){
		// 如果没有回车，则恢复内存中的数值
		LPCTSTR pszCtrlName = msg.pSender->GetName().GetData();		
		CString str;
		if (_tcscmp(pszCtrlName, _T("edit_left")) == 0)
		{
			if (edit_padding_left_){
				str.Format(_T("%d"), (int)margin_left_);
				edit_padding_left_->SetText(str);
			}
		}
		else if (_tcscmp(pszCtrlName, _T("edit_right")) == 0)
		{
			if (edit_padding_right_){
				str.Format(_T("%d"), (int)margin_right_);
				edit_padding_right_->SetText(str);
			}
		}
		else if (_tcscmp(pszCtrlName, _T("edit_top")) == 0)
		{
			if (edit_padding_top_){
				str.Format(_T("%d"), (int)margin_top_);
				edit_padding_top_->SetText(str);
			}
		}
		else if (_tcscmp(pszCtrlName, _T("edit_bottom")) == 0)
		{
			if (edit_padding_bottom_){
				str.Format(_T("%d"), (int)margin_bottom_);
				edit_padding_bottom_->SetText(str);
			}
		}
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT){
		// 响应页边距类型的选择事件
		LPCTSTR pszCtrlName = msg.pSender->GetName().GetData();

		if (_tcscmp(pszCtrlName, _T("combo_padding")) == 0){
			CComboUI* pCombox = (CComboUI*)msg.pSender;
			int nsel = pCombox->GetCurSel();

			cef_pdf_print_margin_type_t curr_type = (cef_pdf_print_margin_type_t)nsel;
			padding_box_->SetVisible(curr_type == PDF_PRINT_MARGIN_CUSTOM);
			if (margin_type_ != curr_type){
				margin_type_ = curr_type;
				PostMessage(WM_PRINT_PREVIEW);
			}
		}
// 		else if (_tcscmp(pszCtrlName, _T("combo_paper")) == 0){
// 			CComboUI* pCombox = (CComboUI*)msg.pSender;
// 			if (pCombox){
// 				int nsel = pCombox->GetCurSel();
// 				Paper* paper = (Paper*)pCombox->GetItemAt(nsel)->GetTag();
// 				if (paper){
// 					page_width_ = paper->GetPaperWidth();
// 					page_height_ = paper->GetPaperHeight();
// 
// 					if (window_loaded){
// 						PostMessage(WM_PRINT_PREVIEW);
// 					}
// 				}
// 			}
// 		}
	}
	else if (msg.sType == _T("textchanged"))
	{
		// 验证输入页边距的合法性
		LPCTSTR pszCtrlName = msg.pSender->GetName().GetData();

		if (_tcscmp(pszCtrlName, _T("edit_left")) == 0){
			CString strValue = edit_padding_left_->GetText();
			PaddingValidate(edit_padding_left_, strValue);
		}
		else if (_tcscmp(pszCtrlName, _T("edit_right")) == 0){
			CString strValue = edit_padding_right_->GetText();
			PaddingValidate(edit_padding_right_, strValue);
		}
		else if (_tcscmp(pszCtrlName, _T("edit_top")) == 0){
			CString strValue = edit_padding_top_->GetText();
			PaddingValidate(edit_padding_top_, strValue);
		}
		else if (_tcscmp(pszCtrlName, _T("edit_bottom")) == 0){
			CString strValue = edit_padding_bottom_->GetText();
			PaddingValidate(edit_padding_bottom_, strValue);
		}
	}
}

void CWndPrint::InitWindow()
{
	combo_padding_ = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("combo_padding")));
	combo_paper_ = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("combo_paper")));

	edit_padding_left_ = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_left")));
	edit_padding_right_ = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_right")));
	edit_padding_top_ = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_top")));
	edit_padding_bottom_ = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_bottom")));

	opt_header_footer_ = static_cast<COptionUI*>(m_pm.FindControl(_T("option_header_footer")));
	btn_ajust_ = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_ajust")));
	padding_box_ = static_cast<CControlUI*>(m_pm.FindControl(_T("padding_box")));
	loading_box_ = static_cast<CControlUI*>(m_pm.FindControl(_T("loading_box")));
	browser_box_ = static_cast<CControlUI*>(m_pm.FindControl(_T("browser_box")));
	loading_tip_ = static_cast<CControlUI*>(m_pm.FindControl(_T("loading_tip")));
	loading_tip_error_ = static_cast<CControlUI*>(m_pm.FindControl(_T("loading_tip_error")));
	
	if (padding_box_){
		padding_box_->SetVisible(false);
	}
		
	CString str;
	if (edit_padding_left_){
		str.Format(_T("%d"), (int)margin_left_);
		edit_padding_left_->SetText(str);
	}
	if (edit_padding_right_){
		str.Format(_T("%d"), (int)margin_right_);
		edit_padding_right_->SetText(str);
	}
	if (edit_padding_top_){
		str.Format(_T("%d"), (int)margin_top_);
		edit_padding_top_->SetText(str);
	}
	if (edit_padding_bottom_){
		str.Format(_T("%d"), (int)margin_bottom_);
		edit_padding_bottom_->SetText(str);
	}
	if (combo_padding_){
		combo_padding_->SelectItem(margin_type_);
	}
	if (opt_header_footer_){
		opt_header_footer_->Selected(header_footer_enabled_);
	}

	//////////////////////////////////////////////////////////////////////////
// 	// 纸张
// 	int sel_index = 0;
// 	for(auto it=paper_list_.begin(); it!=paper_list_.end(); it++){
// 		CListLabelElementUI* elem = new CListLabelElementUI();
// 		str = "A4";
// 		elem->SetText((*it)->GetPaperName().c_str());
// 		elem->SetTag((UINT_PTR)(*it));
// 		combo_paper_->Add(elem);
// 		if ((*it)->IsSelected()){
// 			sel_index = it - paper_list_.begin();
// 		}
// 	}
// 	combo_paper_->SelectItem(sel_index);
	//////////////////////////////////////////////////////////////////////////

	// 初始化完成
	window_loaded = true;
}

void CWndPrint::StartPrintPreview()
{
	if (loading_box_){
		loading_box_->SetVisible(true);
	}
	if (browser_box_){
		browser_box_->SetVisible(false);
	}
	
	if (loading_tip_){
		loading_tip_->SetVisible(true);
	}
	if (loading_tip_error_){
		loading_tip_error_->SetVisible(false);
	}

	//////////////////////////////////////////////////////////////////////////
	// 通过browser生成pdf
	wstring tmp_dir = Configurations::getInstance()->GetTempDir();
	CreateMultipleDirectory(tmp_dir.c_str());
	//得到当前时间
	SYSTEMTIME st;
	::GetLocalTime(&st);
	wchar_t file_name[1000] = { 0 };
	swprintf_s(file_name, _T("%04d%02d%02d%02d%02d%02d%03d.pdf"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	target_path_ = tmp_dir + file_name;

	if (pdf_creator_){

		CefPdfPrintSettings settings = GetPdfPrintSetting();

		pdf_creator_->PrintToPdf(target_path_, settings);
	}
	else{
		pdf_creator_ = new CPDFCreation(this->GetHWND(), html_url_, identity_url_, target_path_, this);
	}
	//////////////////////////////////////////////////////////////////////////
}

void CWndPrint::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{
	wstring frame_url = frame->GetURL();
	if (frame != NULL && frame_url == identity_url_){

		CefPdfPrintSettings settings = GetPdfPrintSetting();

		pdf_creator_->PrintToPdf(target_path_, settings);
	}	
}

CefPdfPrintSettings CWndPrint::GetPdfPrintSetting()
{
	CefPdfPrintSettings settings;
	// Show the URL in the footer.
	settings.header_footer_enabled = header_footer_enabled_;
	CefString(&settings.header_footer_url) = identity_url_;
	CefString(&settings.header_footer_title) = identity_title_;

	settings.margin_type = margin_type_;
	if (margin_type_ == PDF_PRINT_MARGIN_CUSTOM){
		settings.margin_left = margin_left_;
		settings.margin_right = margin_right_;
		settings.margin_top = margin_top_;
		settings.margin_bottom = margin_bottom_;
	}
	int dd = settings.page_width;
	int sd = settings.page_height;
	// 以下参数不支持用户设置
// 	settings.page_width = 0;// page_width_ * 1000;
// 	settings.page_height = 0;//page_height_ * 1000;
	settings.backgrounds_enabled = background_enabled_;
	return settings;
}

void CWndPrint::OnPrepare()
{
	
}

void CWndPrint::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	pdf_creator_->NavigateString();
}

void CWndPrint::OnPdfPrintFinished(const std::wstring& file_path, bool ok)
{
	CGLogger::Info("File \"" + WStringToString(file_path) +"\" " + (ok ? "saved successfully." : "failed to save."));
	try {
		if (ok){
		
			m_pBrowserUI->SetRedirectEnabled(true);
			// load pdf file to browser
			if (m_pBrowserUI){
				m_pBrowserUI->Navigate2(file_path);
			}
		}
		else{
			if (loading_tip_){
				loading_tip_->SetVisible(false);
			}
			if (loading_tip_error_){
				loading_tip_error_->SetVisible(true);
			}
			MessageBox(this->GetHWND(), L"生成预览文件失败！请重新打印", L"Error", 0);
		}

		if (loading_box_){
			loading_box_->SetVisible(false);
		}
		if (browser_box_){
			browser_box_->SetVisible(true);
		}
	}
	catch(std::exception& ex){
		CGLogger::Error("error occured after generating pdf finished. Msg: ", ex.what());
	}

	last_pdf_file_ = display_pdf_file_;
	display_pdf_file_ = file_path;
	RemovePdfFile(last_pdf_file_);
	last_pdf_file_.clear();
}

LPCTSTR CWndPrint::GetWindowClassName() const 
{ 
	return _T("UISubFrame"); 
}

CDuiString CWndPrint::GetSkinFile()
{
	return _T("print_preview.xml");
}

LRESULT CWndPrint::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PRINT_PREVIEW:
		CGLogger::Debug("call StartPrintPreview from postmessage...");
		StartPrintPreview();
	default:
		break;
	}
	return __super::HandleMessage(uMsg,wParam,lParam);
}

CControlUI* CWndPrint::CreateControl(LPCTSTR pstrClass, CPaintManagerUI *pManager)
{
	if( _tcsicmp(pstrClass, _T("Cef")) == 0){
		return m_pBrowserUI;
	}
	return NULL;
}

void CWndPrint::RemovePdfFile(const wstring& path)
{
	DeleteFile(path.c_str());
}

void CWndPrint::PaddingValidate(CEditUI* edit_ctrl, CString &strValue)
{
	if (strValue.GetLength() == 0){
		strValue = "0";
		edit_ctrl->SetText(strValue);
		edit_ctrl->SetSel(strValue.GetLength(),strValue.GetLength());//重设给光标设置位置
		return;
	}
	
	for ( int nIndex = 0; nIndex < strValue.GetLength() ; nIndex++ )
	{
		if( strValue.GetAt( nIndex ) > 57 || strValue.GetAt( nIndex ) < 48 )
		{
			//ASCII码48~57是数字，所以大于57的小于48的都过滤掉
			strValue = strValue.Left(nIndex) + strValue.Right(strValue.GetLength() - nIndex - 1);//包含中文的字符过滤掉
			edit_ctrl->SetText(strValue);
			edit_ctrl->SetSel(strValue.GetLength(),strValue.GetLength());//重设给光标设置位置
		}    
	}

	int num = _ttoi(strValue);
	if (num > kPaddingMaximum){
		strValue.Format(L"%d",kPaddingMaximum);
		edit_ctrl->SetText(strValue);
		edit_ctrl->SetSel(strValue.GetLength(),strValue.GetLength());//重设给光标设置位置
	}
}


//////////////////////////////////////////////////////////////////////////
// ICallbackDelegate events
void CWndPrint::OnSetAddress(const std::wstring& url)
{

}

void CWndPrint::OnSetTitle(const std::wstring& title) 
{
	SetWindowText(m_hWnd, title.c_str());
}

void CWndPrint::OnSetFullscreen(bool fullscreen) 
{

}

void CWndPrint::OnSetLoadingState(bool isLoading,
								  bool canGoBack,
								  bool canGoForward)
{
	if (!isLoading){
		// 禁用pdf里打开页脚链接
		m_pBrowserUI->SetRedirectEnabled(false);
	}
}


//////////////////////////////////////////////////////////////////////////
// ICallbackDelegate methods
void CWndPrint::OnSetDraggableRegions(
	const std::vector<CefDraggableRegion>& regions)
{

}

// Set load error message log
void CWndPrint::OnLoadError(CefRefPtr<CefFrame> frame
								, int errorCode
								, const CefString& errorText
								, const CefString& failedUrl)
{

}

void CWndPrint::OnMinimized()
{
	
}

void CWndPrint::OnExit()
{
	Close(IDOK);
}
//////////////////////////////////////////////////////////////////////////