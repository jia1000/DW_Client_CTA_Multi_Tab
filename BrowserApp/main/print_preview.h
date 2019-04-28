#pragma once
#include "WndFrameBase.h"
#include "base/control/pdf_creator.h"

/************************************************************************/
/*
	CWndPrint´°¿Ú
*/
/************************************************************************/

class CWndPrint
	: public WindowImplBase
	, public ICallbackDelegate
	, public IPdfCreationCallback
{
public:
	CWndPrint(const wstring& url);
	~CWndPrint();

	class Paper{
	public:
		Paper(wstring name, int width, int height, bool selected = false)
		{
			paper_name_ = name;
			paper_size_.Width = width;
			paper_size_.Height = height;
			is_selected_ = selected;
		}
		wstring GetPaperName(){return paper_name_;}
		int GetPaperWidth(){return paper_size_.Width;}
		int GetPaperHeight(){return paper_size_.Height;}
		bool IsSelected(){return is_selected_;}

	private:
		wstring paper_name_;
		Size paper_size_;
		bool is_selected_;
	};

	//////////////////////////////////////////////////////////////////////////
	// WindowImplBase methods
	CDuiString GetSkinFile() OVERRIDE;
	LPCTSTR GetWindowClassName() const OVERRIDE;
	void OnFinalMessage(HWND /*hWnd*/) OVERRIDE;
	void Notify(TNotifyUI& msg) OVERRIDE;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) OVERRIDE;
	CControlUI* CreateControl(LPCTSTR pstrClass, CPaintManagerUI *pManager);
	void InitWindow() OVERRIDE;
	//////////////////////////////////////////////////////////////////////////

	void StartPrintPreview();
	void SetFromUrl(const wstring& from_url);
	void SetFromTitle(const wstring& title);

	//////////////////////////////////////////////////////////////////////////
	// ICallbackDelegate methods
	void OnSetAddress(const std::wstring& url) OVERRIDE;
	// Set the window title.
	void OnSetTitle(const std::wstring& title) OVERRIDE;
	// Set fullscreen mode.
	void OnSetFullscreen(bool fullscreen) OVERRIDE;
	// Set the loading state.
	void OnSetLoadingState(bool isLoading,
		bool canGoBack,
		bool canGoForward) OVERRIDE;
	// Set the draggable regions.
	void OnSetDraggableRegions(
		const std::vector<CefDraggableRegion>& regions) OVERRIDE;
	// Set load error message log
	void OnLoadError(CefRefPtr<CefFrame> frame
		, int errorCode
		, const CefString& errorText
		, const CefString& failedUrl);
	// Minimize window.
	void OnMinimized() OVERRIDE;
	// Exit application.
	void OnExit() OVERRIDE;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// IPdfCreationCallback methods
	void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	void OnPdfPrintFinished(const std::wstring& file_path, bool ok) OVERRIDE;
	void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) OVERRIDE;
	//////////////////////////////////////////////////////////////////////////

private:
	void OnPrepare();
	void RemovePdfFile(const wstring& path);
	void PaddingValidate(CEditUI* edit_ctrl, CString &strValue);
	void ReadParameter();
	void SaveParameter();
	CefPdfPrintSettings GetPdfPrintSetting();

private:
	CCefBrowserUI* m_pBrowserUI;
	string m_sBrowserID;
	wstring html_url_;
	wstring identity_url_;
	wstring identity_title_;
	wstring target_path_;
	wstring preview_path_;
	CPDFCreation* pdf_creator_;
	cef_pdf_print_margin_type_t margin_type_;
	int margin_top_;
	int margin_bottom_;
	int margin_left_;
	int margin_right_;
	bool header_footer_enabled_;
	bool background_enabled_;
	bool landscape;
	int page_width_;
	int page_height_;
	CButtonUI* btn_ajust_;
	CComboBoxUI* combo_paper_;
	CComboBoxUI* combo_padding_;
	CEditUI* edit_padding_left_;
	CEditUI* edit_padding_right_;
	CEditUI* edit_padding_top_;
	CEditUI* edit_padding_bottom_;
	COptionUI* opt_header_footer_;
	CControlUI* padding_box_;
	CControlUI* loading_box_;
	CControlUI* browser_box_;
	CControlUI* loading_tip_;
	CControlUI* loading_tip_error_;
	vector<wstring> pdf_file_paths_;
	wstring last_pdf_file_;
	wstring display_pdf_file_;
	bool window_loaded;
	vector<Paper*> paper_list_;
};