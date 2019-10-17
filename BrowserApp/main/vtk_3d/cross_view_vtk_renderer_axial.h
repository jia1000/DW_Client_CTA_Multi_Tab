/*
 *  
 *  $Id: cross_view_vtk_renderer_axial.h $
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
#include "cross_view_vtk_renderer_base.h"

class WidgetsMprVtk;
class CrossViewVtkActorBase;

class CrossViewVtkRendererAxial : public CrossViewVtkRendererBase {
public:
	static CrossViewVtkRendererAxial *New();

    void SetSlice(int delta_slice);

    

protected:
    CrossViewVtkRendererAxial();
	~CrossViewVtkRendererAxial();

	vtkTypeMacro(CrossViewVtkRendererAxial, vtkRenderer);
	
private:
    vtkSmartPointer<vtkDICOMImageReader> m_v16;
    vtkSmartPointer< CrossViewVtkActorBase> m_main_actor;

    WidgetsMprVtk* m_mpr;
};
