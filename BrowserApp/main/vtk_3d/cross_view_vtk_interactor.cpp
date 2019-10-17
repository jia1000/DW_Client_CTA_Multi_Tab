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
#include "cross_view_vtk_interactor.h"
#include "left_down_vtk_actor.h"
#include "cross_view_vtk_actor.h"

#pragma once
#ifdef __DEPRECATED
#undef __DEPRECATED
#endif 

vtkStandardNewMacro(CrossViewVtkInteractor)

void CrossViewVtkInteractor::SetImageViewer(CrossViewVtkActor* image_actor) {
    _ImageActor = image_actor;
    _Slice = 0;
}

void CrossViewVtkInteractor::SetDicomImageReader(vtkDICOMImageReader* dicomReader)
{
    _DicomReader = dicomReader;
}

void CrossViewVtkInteractor::SetSlice(int slice)
{
    this->Modified();

    _Slice = _ImageActor->SetSlice(slice);
}

void CrossViewVtkInteractor::MoveSliceForward()
{    
    _Slice += 1;
    cout << "MoveSliceForward::Slice = " << _Slice << endl;
    SetSlice(_Slice);   
}

void CrossViewVtkInteractor::MoveSliceBackward()
{
    _Slice -= 1;
    SetSlice(_Slice);    
}

void CrossViewVtkInteractor::OnKeyDown()
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

void CrossViewVtkInteractor::OnMouseWheelForward()
{
    MoveSliceForward();
}

void CrossViewVtkInteractor::OnMouseWheelBackward()
{
    MoveSliceBackward();
}


