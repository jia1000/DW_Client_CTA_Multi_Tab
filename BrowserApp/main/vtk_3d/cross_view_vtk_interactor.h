/*
 *  
 *  $Id: CrossViewActor.h $
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
#ifdef __DEPRECATED
#undef __DEPRECATED
#endif 

#include "global_include.h"

class CrossViewVtkActor;

class CrossViewVtkInteractor : public vtkInteractorStyleImage
{
public:
    static CrossViewVtkInteractor* New();
    vtkTypeMacro(CrossViewVtkInteractor, vtkInteractorStyleImage);

protected:
    //vtkImageViewer2* _ImageViewer;
    CrossViewVtkActor* _ImageActor;
    vtkDICOMImageReader* _DicomReader;
    int _Slice;

public:
    void SetImageViewer(CrossViewVtkActor* image_actor);

    void SetDicomImageReader(vtkDICOMImageReader* dicomReader);

    void SetSlice(int slice);

protected:
    void MoveSliceForward();

    void MoveSliceBackward();

    virtual void OnKeyDown();

    virtual void OnMouseWheelForward();

    virtual void OnMouseWheelBackward();
};


