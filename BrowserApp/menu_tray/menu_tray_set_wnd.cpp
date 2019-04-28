#include "stdafx.h"

#include "menu_tray_set_wnd.h"

#include "encrypt/Utf8String.h"
#include "extended_control/button_hand_ui.h"
#include "fileversion_number.h"
#include "include/cef_version.h"
#include "main/BrowserWndManager.h"
#include "screen/screen_info_controller.h"
#include "ui_option_screen.h"

// 来自于CEF头文件 libcef_dll\include\cef_version.h
#define CEF_API_HASH_PLATFORM_49 "64b27477b82b44b51ce817522f744fca6768cbbb"
#define CEF_API_HASH_PLATFORM_69 "45d12d3e62f23ad13c72890b2a12749f5c6679cd"

DUI_BEGIN_MESSAGE_MAP(MenuTraySet_Wnd, WindowImplBase)
DUI_END_MESSAGE_MAP()


MenuTraySet_Wnd::MenuTraySet_Wnd(void)
{
    
}

MenuTraySet_Wnd::~MenuTraySet_Wnd(void)
{
    
}

DuiLib::CDuiString MenuTraySet_Wnd::GetSkinFile()
{
    return _T("menu_tray\\menu_tray_set.xml");
}

void MenuTraySet_Wnd::OnFinalMessage( HWND hWnd)
{
    __super::OnFinalMessage(hWnd);
    delete this;
    // 设置管理器相关指针为NULL
    TrayWindowManager::getInstance()->DestroyedTraySetWnd();
}

LPCTSTR MenuTraySet_Wnd::GetWindowClassName(void) const
{
    return _T("MenuTraySet_Wnd");
}

CControlUI* MenuTraySet_Wnd::CreateControl(LPCTSTR pstrClass, CPaintManagerUI *pManager)
{
    // 创建自定义的Option控件
    if (_tcsicmp(pstrClass, _T("OptionScreen")) == 0)  {
        return  new COptionUIScreen();  
    }
	// 创建自定义的Button控件
	if (_tcsicmp(pstrClass, _T("ButtonHand")) == 0)  {
		return  new CButtonHandUI();  
	}
    return NULL;
}

void MenuTraySet_Wnd::OnClick(TNotifyUI &msg)
{
    CDuiString sName = msg.pSender->GetName();
}

void MenuTraySet_Wnd::Notify(TNotifyUI &msg)
{
    CDuiString name = msg.pSender->GetName();
    if (msg.sType == _T("click")) {
        LPCTSTR pszCtrlName = msg.pSender->GetName().GetData();
        std::wstring ws_control_name(pszCtrlName);
        if (_tcscmp(pszCtrlName, _T("btn_sysclose")) == 0) {
            this->Close();
        } else if (_tcscmp(pszCtrlName, _T("btn_screen_identification")) == 0) {
            //更新全部屏幕
            RefreshScreenListUI(true);
        } else if (_tcscmp(pszCtrlName, _T("btn_screen_zoom_reset")) == 0) {
            // 重置缩放倍数为1.0  随后需要考虑下缩放数组的处理问题
			TrayWindowManager::getInstance()->NotifyZoomIndexChanged(4);
            
        } else if(std::wstring::npos != ws_control_name.find(L"screen_option")){
            std::wstring cur_click_screen(L"")  ;   
            m_screen_shots = ScreenInfoController::GetInstance()->GetScreenShot(); 
            if (m_screen_shots.size() > 0) {
                bool is_find = IsNormalOfDisplay(std::wstring(pszCtrlName), cur_click_screen);
                if (is_find) {
                    UpdateScreensShowText(cur_click_screen);
                } 
            }
        }
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
                if (item_name.CompareNoCase(L"screen_set") == 0) {
                	TabSwitchSelected(TREE_NODE_SCREEN_FIXED);
                } else {
                    TabSwitchSelected(TREE_NODE_SCREEN_ABOUT);
                }
            } else {
                // 如果是二级菜单
                if (item_name.CompareNoCase(L"screen_fixed") == 0) {
                    TabSwitchSelected(TREE_NODE_SCREEN_FIXED);
                } else if (item_name.CompareNoCase(L"screen_zoom") == 0) {
                    TabSwitchSelected(TREE_NODE_SCREEN_ZOOM);
                } else if (item_name.CompareNoCase(L"chrome_core_about") == 0) {
                    TabSwitchSelected(TREE_NODE_SCREEN_ABOUT);
                } 
                // 其它二级菜单
            }
        }
    } else if (msg.sType == _T("valuechanged")){ 
        LPCTSTR pszCtrlName = msg.pSender->GetName().GetData();
        if (_tcscmp(pszCtrlName, _T("slider_screen_zoom")) == 0) {
            CSliderUI * pSlider = static_cast<CSliderUI*>(msg.pSender);
            if (pSlider) {
                int index = pSlider->GetValue();           
                TrayWindowManager::getInstance()->NotifyZoomIndexChanged(index);
            }
        }
    }
    return WindowImplBase::Notify(msg);
}

void MenuTraySet_Wnd::SetSliderIndex(int index)
{
    CSliderUI* pSlider = static_cast<CSliderUI*>(m_pm.FindControl(_T("slider_screen_zoom")));
    if (pSlider) {
        pSlider->SetValue(index);
    }
}

void MenuTraySet_Wnd::SetZoomButtonValue(double value)
{
    CButtonUI* pZoomCur = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_screen_zoom_cur")));
    if (pZoomCur) {
		CDuiString text;
		text.Format(_T("%d%%"), (int)(value * 100));
        pZoomCur->SetText(text);
    }
}

void MenuTraySet_Wnd::TabSwitchSelected(ENUM_TREE_NODE_SELECTED select_index)
{
    CTabLayoutUI* pTab = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tab_switch")));
    if (pTab) {
        pTab->SelectItem(select_index);

        CGroupBoxUI* pGroup = static_cast<CGroupBoxUI*>(m_pm.FindControl(_T("group_box_right")));
        CLabelUI* pLabel = static_cast<CLabelUI*>(m_pm.FindControl(_T("set_tip_info")));
        
        if (pGroup && pLabel) {
	        if (select_index == TREE_NODE_SCREEN_SET || select_index == TREE_NODE_SCREEN_FIXED) {
	            pGroup->SetText(_T("固定屏幕"));
                pLabel->SetVisible(true);
            } else if (select_index == TREE_NODE_SCREEN_ZOOM) {
                pGroup->SetText(_T("显示内容放大倍率"));
                pLabel->SetVisible(false);
            }else if(select_index >= TREE_NODE_SCREEN_ABOUT && select_index < TREE_NODE_SELECTED_COUNT){
                pLabel->SetVisible(false);
                pGroup->SetText(_T("软件配置信息"));
                // 内核版本号
                CLabelUI* pLabelCoreVersion = static_cast<CLabelUI*>(m_pm.FindControl(_T("core_version_label")));
	            if (pLabelCoreVersion) {
                    std::wstring core_vertion = GetChromeCoreVersionText();
                    pLabelCoreVersion->SetText(core_vertion.c_str());
                }
                // 客户端软件版本号
                CLabelUI* pLabelClientVersion = static_cast<CLabelUI*>(m_pm.FindControl(_T("client_version_label")));
                if (pLabelClientVersion) {
                    std::string file_version = IDS_FILEVERSION_VALUE_V;
                    std::wstring ws_file_version = Utf8ToUnicode(file_version);                    
                    pLabelClientVersion->SetText(ws_file_version.c_str());
                }
	        }
        }
    }
}

void MenuTraySet_Wnd::InitWindow()
{
    m_pTagsListUI = static_cast<CListUI*>(m_pm.FindControl(_T("screen_list"))); 

    TabSwitchSelected(TREE_NODE_SCREEN_FIXED);

    // TreeNode的选中高亮显示
    CTreeNodeUI* pNode = static_cast<CTreeNodeUI*>(m_pm.FindControl(_T("screen_fixed")));
    if (pNode) {
        pNode->Select(true);
    }

    // 初始化窗口时，“屏幕列表”使用空图片占位
    RefreshScreenListUI(false);

	// 设置浏览器放大器倍数
	int slide_index = Configurations::getInstance()->GetBrowserZoomLevelIndex();
	TrayWindowManager::getInstance()->NotifyZoomIndexChanged(slide_index);
}

LRESULT MenuTraySet_Wnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return __super::HandleMessage(uMsg,wParam,lParam);
}

std::wstring MenuTraySet_Wnd::GetChromeCoreVersionText()
{
    std::wstring cef_core(L"");
    if (0 == strcmp(CEF_API_HASH_PLATFORM, CEF_API_HASH_PLATFORM_49)) {
        cef_core = L"49 内核";
    } else if (0 == strcmp(CEF_API_HASH_PLATFORM, CEF_API_HASH_PLATFORM_69)) {
        cef_core = L"69 内核";
    }
    return cef_core;
}

void MenuTraySet_Wnd::RefreshScreenListUI(bool is_force)
{           
    if (!m_pTagsListUI) {
        return;
    }

    // 删除duilib管理器中“屏幕列表”的图片资源    
    for (int Index = 0; Index < m_pTagsListUI->GetCount() ; Index++) {
        CListContainerElementUI *pEle = (CListContainerElementUI* )m_pTagsListUI->GetItemAt(Index);
        if (pEle) {
            CLabelUI *p_screen_option = static_cast<CLabelUI *>(m_pm.FindSubControlByName(pEle, _T("screen_img_label")));
            if (p_screen_option) {
                LPCTSTR image_name = p_screen_option->GetBkImage();
                m_pm.RemoveImage(image_name);
            }
        }
    }
    // 清空“屏幕列表”的控件
	m_pTagsListUI->RemoveAll();
	
    std::wstring cur_select_screen = ScreenInfoController::GetInstance()->DetectionDeafaultScreenName();        
    m_screen_shots = ScreenInfoController::GetInstance()->GetScreenShot();    
    auto iter = m_screen_shots.begin();
    CDialogBuilder builder;

	for (size_t i = 0; i < m_screen_shots.size(); ++i, ++iter) {
		CListContainerElementUI* pEle = NULL;

		if(!builder.GetMarkup()->IsValid()) {
		    pEle = static_cast<CListContainerElementUI *>(builder.Create(_T("menu_tray\\menu_tray_screen_item.xml"), NULL, this, &m_pm));
        }
		else {
		    pEle = static_cast<CListContainerElementUI*>(builder.Create(this,&m_pm));
        }
		
        if (!pEle) {
            continue;
        }

		COptionUIScreen *p_screen_option = static_cast<COptionUIScreen *>(m_pm.FindSubControlByName(pEle, (_T("screen_option"))));

		if (p_screen_option) {	
            std::wstring ws = GetShowText(i);
            if (iter->first == cur_select_screen) {
                ws += _T("(默认)");
                p_screen_option->Selected(true);
            }
            std::wstring option_name = GetControlNameText(iter->first);
            p_screen_option->SetName(option_name.c_str());
            p_screen_option->SetText(ws.c_str());
		}

		CLabelUI* screen_img_label = static_cast<CLabelUI *>(m_pm.FindSubControlByName(pEle,(_T("screen_img_label"))));

		if (screen_img_label) {
		    if (is_force) {
		    	screen_img_label->SetBkImage(iter->second.c_str());
		    } else {
                screen_img_label->SetBkImage(_T(""));
		    }
		}
		    
		m_pTagsListUI->Add(pEle);
	} 

    // 设置在屏幕数量小于等于2时，不显示滚动条。 原因：  调整xml的控件大小，不太成功。
    if (m_pTagsListUI->GetCount() <= 2) {
        m_pTagsListUI->SetAttribute(_T("vscrollbar"), _T("false"));
    } else {
        m_pTagsListUI->SetAttribute(_T("vscrollbar"), _T("true"));
    }    
}

void MenuTraySet_Wnd::SetSelectedScreenIndex(size_t shot_index)
{    
    if (shot_index >=0 && shot_index < m_screen_shots.size()) {
        auto iter = m_screen_shots.begin();
        for (int index = 0; iter != m_screen_shots.end(); ++index, ++iter) {
            if (index == shot_index) {
                ScreenInfoController::GetInstance()->SetSelectedScreen(iter->first);
            }
        }
    }
}

std::wstring MenuTraySet_Wnd::GetControlNameText(std::wstring display_name)
{
    std::wstring option_name = std::wstring(_T("screen_option")) + std::wstring(_T("_")) + display_name;

    return option_name;
}

std::wstring MenuTraySet_Wnd::GetShowText(int index)
{
    std::stringstream ss; 
    ss << char('A' + index);
    std::wstring ws1 = StringToWString(ss.str());
    std::wstring ws2(_T("显示器"));
    std::wstring ws = ws1 + ws2;

    return ws;
}

bool MenuTraySet_Wnd::IsNormalOfDisplay(std::wstring control_name, std::wstring& display_name)
{
    m_screen_shots = ScreenInfoController::GetInstance()->GetScreenShot(); 

    if (m_screen_shots.size() <= 0) {
        return false;
    }

    for (auto iter = m_screen_shots.begin(); iter !=  m_screen_shots.end(); ++iter) {	
        std::wstring option_name = GetControlNameText(iter->first);
        if (option_name == control_name) {
            display_name = iter->first;
            return true;
        }
    }    

    return false;
}

void MenuTraySet_Wnd::UpdateScreensShowText(std::wstring cur_click_screen)
{
    if (m_pTagsListUI) {
        for (int i = 0; i < m_pTagsListUI->GetCount() ; i++) {
            CListContainerElementUI *pEle = (CListContainerElementUI* )m_pTagsListUI->GetItemAt(i);
            if (!pEle) {
                continue;
            }

            COptionUIScreen *p_screen_option = static_cast<COptionUIScreen *>(m_pm.FindSubControlByClass(pEle, _T("OptionScreenUI")));
            if (!p_screen_option) {
                continue;
            }

            std::wstring ws_name     = p_screen_option->GetName();
            std::wstring option_name = GetControlNameText(cur_click_screen);
            std::wstring ws          = GetShowText(i);

            if (ws_name == option_name) {
                ws += _T("(默认)");
                //设置控件选中
                p_screen_option->Selected(true);
                //修改配置文件--选中屏幕
                SetSelectedScreenIndex(i);
            } else {
                p_screen_option->Selected(false);
            }
            //设置控件显示文本
            p_screen_option->SetText(ws.c_str());
        }
    }
}