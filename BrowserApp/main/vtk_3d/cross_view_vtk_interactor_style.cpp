/*
 *  
 *  $Id: crossviewvtkinteractor.cpp $
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
#include "cross_view_vtk_interactor_style.h"
#include "left_down_vtk_actor.h"
#include "cross_view_vtk_actor.h"

#include "main/vtk_3d/widgets_mpr_vtk.h"

#pragma once
#ifdef __DEPRECATED
#undef __DEPRECATED
#endif 

vtkStandardNewMacro(CrossViewVtkInteractorStyle)

void CrossViewVtkInteractorStyle::SetImageViewer(WidgetsMprVtk* mpr) {
    //_ImageActor = image_actor;
    m_mpr = mpr;
}

void CrossViewVtkInteractorStyle::SetDicomImageReader(vtkDICOMImageReader* dicomReader)
{
    _DicomReader = dicomReader;
}

void CrossViewVtkInteractorStyle::SetSlice(int slice)
{
    this->Modified();

    int* winSize = this->GetInteractor()->GetRenderWindow()->GetSize();

    int eventStation[3] = { 0 };
    this->GetInteractor()->GetEventPosition(eventStation);
    
    CrossViewVtkActor* actor = NULL;
    if (eventStation[0] < winSize[0] / 2 && eventStation[1] < winSize[1] / 2) {
        actor = m_mpr->sagittal_normal;
    } else if(eventStation[0] > winSize[0] / 2 && eventStation[1] > winSize[1] / 2){
        actor = m_mpr->coronal_normal;
    }else if (eventStation[0] < winSize[0] / 2 && eventStation[1] > winSize[1] / 2) {
        actor = m_mpr->axial_normal;
    } else {
        actor = m_mpr->axial_normal;
    }
    if (actor) {
    	actor->SetSlice(slice);
    }
}

void CrossViewVtkInteractorStyle::MoveSliceForward()
{    
    SetSlice(1);   
}

void CrossViewVtkInteractorStyle::MoveSliceBackward()
{
    SetSlice(-1);    
}

void CrossViewVtkInteractorStyle::OnKeyDown()
{
    std::string key = this->GetInteractor()->GetKeySym();
    if (key.compare("Up") == 0)
        MoveSliceForward();
    else if (key.compare("Down") == 0)
    {
        MoveSliceBackward();
    }

    vtkInteractorStyleImage::OnKeyDown();
}

void CrossViewVtkInteractorStyle::OnMouseWheelForward()
{
    MoveSliceForward();
}

void CrossViewVtkInteractorStyle::OnMouseWheelBackward()
{
    MoveSliceBackward();
}


