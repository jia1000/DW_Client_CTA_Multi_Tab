/*
 *  
 *  $Id: vtkView.h $
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
#pragma once

#include "global_include.h"

// CvtkView

class CvtkView
{


public:
	CvtkView(HWND parent, CButtonUI* vtkShowBtn);
	virtual ~CvtkView();

protected:
	
	virtual void PreSubclassWindow();
public:
	

	void SetImageData(vtkSmartPointer<vtkImageData> ImageData);
	void SetupReslice();
	void SetSliceDirection(int dir) {m_Direction = dir;}
	void Render();

	vtkSmartPointer< vtkImagePlaneWidget > GetImagePlaneWidget() {return m_ImagePlaneWidget;}
	vtkSmartPointer< vtkResliceCursorWidget >    GetResliceCursorWidget() {return m_ResliceCursorWidget;}

	vtkSmartPointer< vtkResliceCursor > GetResliceCursor() {return m_ResliceCursor;}
	void SetResliceCursor(vtkSmartPointer< vtkResliceCursor > cursor);

public:
	vtkSmartPointer<vtkRenderer>    m_Renderer;
	vtkSmartPointer<vtkRenderWindow> m_RenderWindow;

	vtkSmartPointer< vtkImagePlaneWidget >   m_ImagePlaneWidget;
	vtkSmartPointer< vtkResliceCursorWidget> m_ResliceCursorWidget;
	vtkSmartPointer< vtkResliceCursor >      m_ResliceCursor;
	vtkSmartPointer< vtkResliceCursorThickLineRepresentation > m_ResliceCursorRep;

	vtkSmartPointer<vtkImageData> m_ImageData;
	int m_Direction;

	HWND m_parentWnd;
	CButtonUI* m_vtkShowBtn;
};


