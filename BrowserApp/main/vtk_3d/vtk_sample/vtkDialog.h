#pragma once
#include <UIlib.h>
using namespace DuiLib;

//////////////////////////////////////////////////////////////////////////
// 该类作为学习Duilib各种控件的入口窗口
//////////////////////////////////////////////////////////////////////////

class CvtkView;
class CvtkView3D;

#include "main/vtk_3d/vtk_sample/vtkView.h"
#include "main/vtk_3d/vtk_sample/vtkView3D.h"

class VtkDialog : public WindowImplBase
{
public:
	VtkDialog(void);
	virtual ~VtkDialog(void);

	virtual LPCTSTR GetWindowClassName() const ;
	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual CControlUI* CreateControl(LPCTSTR pstrClass, CPaintManagerUI *pManager);

	virtual void    Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void SetXXXXXXXXXXXXXXXXX();
private:
	CvtkView*   m_AxialView;
	CvtkView*   m_SagittalView;
	CvtkView*   m_CoronalView;
	CvtkView3D* m_3DView;

};

