/*
 *  
 *  $Id: vtkView3D.cpp $
 *  DeepWise 
 *
 *  Copyright 2017-2030 DeepWise All rights reserved.
 *  http://www.deepwise.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *  Code based in VTK
 */
// vtkView3D.cpp : implementation file
//

#include "stdafx.h"
//#include "vtkDialog.h"
#include "vtkView3D.h"


// CvtkView3D

CvtkView3D::CvtkView3D(HWND parent, CButtonUI* vtkShowBtn)
	: m_parentWnd(parent)
	, m_vtkShowBtn(vtkShowBtn)
{
	PreSubclassWindow();
}

CvtkView3D::~CvtkView3D()
{
}

void CvtkView3D::Render()
{
	m_Renderer->ResetCamera();
	m_RenderWindow->Render();
}

void CvtkView3D::PreSubclassWindow()
{
	RECT rect;
	if (m_vtkShowBtn) {
		rect = m_vtkShowBtn->GetClientPos();
		m_vtkShowBtn->SetText(_T("Please wait..."));
	}

	m_Renderer = vtkSmartPointer<vtkRenderer>::New();

	m_RenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	m_RenderWindow->SetParentId(this->m_parentWnd);
	m_RenderWindow->SetSize(rect.right - rect.left, rect.bottom - rect.top);
	m_RenderWindow->SetPosition(rect.left, rect.top);
	m_RenderWindow->AddRenderer(m_Renderer);

	if(m_RenderWindow->GetInteractor() == NULL)
	{
		vtkSmartPointer<vtkRenderWindowInteractor> RenderWindowInteractor =
			vtkSmartPointer<vtkRenderWindowInteractor>::New();
		RenderWindowInteractor->SetRenderWindow(m_RenderWindow);
		RenderWindowInteractor->Initialize();
	}

}