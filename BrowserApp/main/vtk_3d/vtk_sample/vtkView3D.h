/*
 *  
 *  $Id: vtkView3D.h $
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

#include "vtkSmartPointer.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
// CvtkView3D

class CvtkView3D
{


public:
	CvtkView3D(HWND parent, CButtonUI* vtkShowBtn);
	virtual ~CvtkView3D();



protected:
	virtual void PreSubclassWindow();

public:
	
	void Render();

	vtkSmartPointer<vtkRenderWindowInteractor> GetInteractor() {return m_RenderWindow->GetInteractor();}
	vtkSmartPointer<vtkRenderer> GetRenderer() {return m_Renderer;}


public:
	vtkSmartPointer<vtkRenderer>     m_Renderer;
	vtkSmartPointer<vtkRenderWindow> m_RenderWindow;

	HWND m_parentWnd;
	CButtonUI* m_vtkShowBtn;
};


