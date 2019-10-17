/*
 *  
 *  $Id: cross_view_vtk_renderer_axial.cpp $
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

#include "stdafx.h"
#include "cross_view_vtk_renderer_axial.h"
#include "cross_view_vtk_actor.h"
#include "widgets_mpr_vtk.h"

vtkStandardNewMacro(CrossViewVtkRendererAxial);

CrossViewVtkRendererAxial::CrossViewVtkRendererAxial()
{
    double leftViewStation[4] = { 0.0, 0.5, 0.5, 1.0 };
    SetBackground(0.1, 0.2, 0.4);

    vtkCamera* cam1 = GetActiveCamera();
    cam1->SetFocalPoint(0, 0, 0);
    cam1->SetPosition(0, 0, 1);
    cam1->SetViewUp(0, 1, 0);
    ResetCamera();

    DrawOn();
    SetViewport(leftViewStation);
    
}


CrossViewVtkRendererAxial::~CrossViewVtkRendererAxial()
{	
}


void CrossViewVtkRendererAxial::SetSlice(int delta_slice)
{
    

    return ;
}
