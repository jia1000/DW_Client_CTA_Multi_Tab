#include "cef_client_handler.h"
#include "include/cef_base.h"
#include "include/cef_load_handler.h"
#include <vector>
#include <map>


class ICallbackDelegate {
public:
	//// Called when the browser has been created.
	//virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) = 0;

	//// Called when the BrowserWindow has been destroyed.
	//virtual void OnBrowserWindowDestroyed() = 0;

	// Set the window URL address.
	virtual void OnSetAddress(const std::wstring& url) = 0;

	// Set the window title.
	virtual void OnSetTitle(const std::wstring& title) = 0;

	// Set fullscreen mode.
	virtual void OnSetFullscreen(bool fullscreen) = 0;

	// Set the loading state.
	virtual void OnSetLoadingState(bool isLoading,
		bool canGoBack,
		bool canGoForward) = 0;

	// Set the draggable regions.
	virtual void OnSetDraggableRegions(
		const std::vector<CefDraggableRegion>& regions) = 0;
	
	// Set load error message log
	void OnLoadError(CefRefPtr<CefFrame> frame
		, int errorCode
		, const CefString& errorText
		, const CefString& failedUrl);

	// Minimize window.
	virtual void OnMinimized() = 0;

	// Close current window
	virtual void OnExit() = 0;

	// 
	virtual void OnZoomValueChanged(int step) {};

protected:
	virtual ~ICallbackDelegate() {}
};