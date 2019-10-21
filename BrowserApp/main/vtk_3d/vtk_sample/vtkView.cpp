/*
 *  
 *  $Id: vtkView.cpp $
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
// vtkView.cpp : implementation file
//

#include "stdafx.h"
//#include "vtkDialog.h"
#include "vtkView.h"

#include "vtkRenderWindowInteractor.h"

// CvtkView


CvtkView::CvtkView(HWND parent, CButtonUI* vtkShowBtn)
	: m_parentWnd(parent)
	, m_vtkShowBtn(vtkShowBtn)
{
	m_ImageData = NULL;
	m_Direction = 0;

	PreSubclassWindow();
}

CvtkView::~CvtkView()
{
}

void CvtkView::PreSubclassWindow()
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

	m_ImagePlaneWidget = vtkSmartPointer< vtkImagePlaneWidget >::New();

	m_ResliceCursor = vtkSmartPointer< vtkResliceCursor >::New();
	m_ResliceCursorRep = vtkSmartPointer< vtkResliceCursorThickLineRepresentation >::New();
	m_ResliceCursorRep->GetResliceCursorActor()-> 
		GetCursorAlgorithm()->SetResliceCursor(m_ResliceCursor);

	m_ResliceCursorWidget = vtkSmartPointer< vtkResliceCursorWidget>::New();
	m_ResliceCursorWidget->SetInteractor(m_RenderWindow->GetInteractor());   
    m_ResliceCursorWidget->SetDefaultRenderer(m_Renderer);  
	m_ResliceCursorWidget->SetRepresentation(m_ResliceCursorRep);


	m_RenderWindow->Start();
}


void CvtkView::SetImageData(vtkSmartPointer<vtkImageData> ImageData)
{
	if (ImageData == NULL ) return;

	m_ImageData = ImageData;
	SetupReslice();
	Render();
}
void CvtkView::SetupReslice()
{
	if (m_ImageData == NULL) return;
	int dims[3];
	m_ImageData->GetDimensions(dims);

	//////////////////////////////////////////////////////////////////////////
	m_ImagePlaneWidget->SetInputData(m_ImageData); 
	m_ImagePlaneWidget->SetPlaneOrientation(m_Direction); 
	m_ImagePlaneWidget->SetSliceIndex(dims[m_Direction]/2); 
	m_ImagePlaneWidget->On(); 
	m_ImagePlaneWidget->InteractionOn(); 

	//////////////////////////////////////////////////////////////////////////
	m_ResliceCursor->SetCenter(m_ImageData->GetCenter()); 
	m_ResliceCursor->SetImage(m_ImageData); 
	m_ResliceCursor->SetThickMode(0); 

	m_ResliceCursorRep->GetResliceCursorActor()-> 
		GetCursorAlgorithm()->SetResliceCursor(m_ResliceCursor);
	m_ResliceCursorRep->GetResliceCursorActor()-> 
		GetCursorAlgorithm()->SetReslicePlaneNormal(m_Direction);

	m_ResliceCursorWidget->SetEnabled(1); 

	//////////////////////////////////////////////////////////////////////////
	if (2 == m_Direction) {			//axial
		vtkCamera* cam1 = m_Renderer->GetActiveCamera();
		cam1->SetFocalPoint(0, 0, 0);
		cam1->SetPosition(0, 0, 1);
		cam1->SetViewUp(0, 1, 0);
		m_Renderer->ResetCamera();
	} else if(1 == m_Direction){    // coronal
		vtkCamera* cam2 = m_Renderer->GetActiveCamera();
		cam2->SetFocalPoint(0, 0, 0);
		cam2->SetPosition(0, -1, 0);
		cam2->SetViewUp(0, 0, -1);
		m_Renderer->ResetCamera();
	} else if(0 == m_Direction)    // sagittal
	{	
		vtkCamera* cam3 = m_Renderer->GetActiveCamera();
		cam3->SetFocalPoint(0, 0, 0);
		cam3->SetPosition(-1, 0, 0);
		cam3->SetViewUp(0, 0, -1);
		m_Renderer->ResetCamera();
	}
	//////////////////////////////////////////////////////////////////////////

	m_Renderer->ResetCamera(); 

	//////////////////////////////////////////////////////////////////////////
	double range[2]; 
	m_ImageData->GetScalarRange(range); 
	m_ResliceCursorWidget->GetResliceCursorRepresentation()->
		SetWindowLevel(range[1]-range[0], (range[0]+range[1])/2.0); 
	m_ImagePlaneWidget->SetWindowLevel(range[1]-range[0], (range[0]+range[1])/2.0); 

	
}

void CvtkView::SetResliceCursor(vtkSmartPointer< vtkResliceCursor > cursor)
{
	m_ResliceCursor = cursor;
}

void CvtkView::Render()
{
	m_RenderWindow->Render();
}