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
class WidgetsMprVtk;

class CrossViewVtkInteractorStyle : public vtkInteractorStyleImage
{
public:
    static CrossViewVtkInteractorStyle* New();
    vtkTypeMacro(CrossViewVtkInteractorStyle, vtkInteractorStyleImage);

protected:
    //vtkImageViewer2* _ImageViewer;
    //CrossViewVtkActor* _ImageActor;
    WidgetsMprVtk* m_mpr;
    vtkDICOMImageReader* _DicomReader;
    int _Slice;

public:
    void SetImageViewer(WidgetsMprVtk* mpr);

    void SetDicomImageReader(vtkDICOMImageReader* dicomReader);

    void SetSlice(int slice);

protected:
    void MoveSliceForward();

    void MoveSliceBackward();

    virtual void OnKeyDown();

    virtual void OnMouseWheelForward();

    virtual void OnMouseWheelBackward();
};

