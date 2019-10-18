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
class WidgetsMprVtk2;

enum
{
    SLICE_ORIENTATION_AXIAL     = 2,   // XY
    SLICE_ORIENTATION_CORONAL   = 1,   // XZ
    SLICE_ORIENTATION_SAGITTAL  = 0    // YZ
};

class CrossViewVtkActorBase : public vtkImageActor {
public:
	static CrossViewVtkActorBase *New();

    virtual void SetSlice(int delta_slice);
	
    virtual void SetRenerWindow(vtkRenderWindow* renwindow);
    // 这个函数，需要放在其他几个Set之后
    virtual void SetInputConnection(vtkSmartPointer<vtkDICOMImageReader> v16);

    virtual void UpdateDisplay();

protected:
    CrossViewVtkActorBase();
	~CrossViewVtkActorBase();

	vtkTypeMacro(CrossViewVtkActorBase, vtkImageActor);
	
    vtkSmartPointer<vtkDICOMImageReader> m_v16;
    vtkSmartPointer<vtkRenderWindow> m_renderWindow;

    std::string m_actor_name;

    int m_data_extent[6];

    int max_slice;
    int min_slice;
    int m_cur_cross_normal;
    int SliceOrientation;
};

/////////////////////////////////////////////
class CrossViewVtkActorAxial : public CrossViewVtkActorBase {
public:
    static CrossViewVtkActorAxial* New();    

protected:
    CrossViewVtkActorAxial();
    ~CrossViewVtkActorAxial();

    vtkTypeMacro(CrossViewVtkActorAxial, CrossViewVtkActorBase);

private:
    
};
//////////////////////////////////////////////////////

class CrossViewVtkActorCoronal : public CrossViewVtkActorBase {
public:
    static CrossViewVtkActorCoronal* New();

protected:
    CrossViewVtkActorCoronal();
    ~CrossViewVtkActorCoronal();

    vtkTypeMacro(CrossViewVtkActorCoronal, CrossViewVtkActorBase);

private:

};
//////////////////////////////////////////////////////

class CrossViewVtkActorSagittal : public CrossViewVtkActorBase {
public:
    static CrossViewVtkActorSagittal* New();

protected:
    CrossViewVtkActorSagittal();
    ~CrossViewVtkActorSagittal();

    vtkTypeMacro(CrossViewVtkActorSagittal, CrossViewVtkActorBase);

private:

};
//////////////////////////////////////////////////////
