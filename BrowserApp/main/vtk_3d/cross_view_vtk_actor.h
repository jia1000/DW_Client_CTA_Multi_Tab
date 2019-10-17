/*
 *  
 *  $Id: CrossViewVtkActor.h $
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

class WidgetsMprVtk;

enum
{
    SLICE_ORIENTATION_AXIAL     = 0,
    SLICE_ORIENTATION_CORONAL   = 1,
    SLICE_ORIENTATION_SAGITTAL  = 2
};

class CrossViewVtkActor : public vtkImageActor {
public:
	static CrossViewVtkActor *New();

    void SetSlice(int delta_slice);
	
    void SetMprWindowControl(WidgetsMprVtk* mpr);


    void SetRendeerWindow(vtkSmartPointer< vtkRenderWindow> renderer_window);

    void SetSliceOrientationMy(int orientation);

    void SetActorName(std::string actor_name);

    // 这个函数，需要放在其他几个Set之后
    virtual void SetInputConnection(vtkSmartPointer<vtkDICOMImageReader> v16);

    void UpdateDisplay();

protected:
    CrossViewVtkActor();
	~CrossViewVtkActor();
	
    

	vtkTypeMacro(CrossViewVtkActor, vtkImageActor);
	
private:
    vtkSmartPointer< vtkRenderWindow> m_renderer_window;
    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkDICOMImageReader> m_v16;

    WidgetsMprVtk* m_mpr;

    std::string m_actor_name;

    int m_data_extent[6];

    int max_slice;
    int min_slice;
    int m_cur_cross_normal;
    int SliceOrientation;
};
