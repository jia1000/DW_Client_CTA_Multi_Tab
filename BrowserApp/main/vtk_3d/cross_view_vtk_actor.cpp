/*
 *  
 *  $Id: CrossViewVtkActor.cpp $
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
#include "cross_view_vtk_actor.h"

#include "widgets_mpr_vtk.h"

vtkStandardNewMacro(CrossViewVtkActorBase);

CrossViewVtkActorBase::CrossViewVtkActorBase()
{
}


CrossViewVtkActorBase::~CrossViewVtkActorBase()
{	
}

void CrossViewVtkActorBase::SetMprWindowControl(WidgetsMprVtk* mpr)
{
    m_mpr = mpr;
}

void CrossViewVtkActorBase::SetInputConnection(vtkSmartPointer<vtkDICOMImageReader> v16)
{
    m_v16 = v16;
    
    vtkSmartPointer<vtkImageMapToColors> sagittalColors = vtkSmartPointer<vtkImageMapToColors>::New();
    sagittalColors->SetInputConnection(m_v16->GetOutputPort());

    this->GetMapper()->SetInputConnection(sagittalColors->GetOutputPort());

    
    int* data_extent;
    data_extent = m_v16->GetDataExtent();

    for (int Index = 0; Index < 6; Index++) {
        m_data_extent[Index] = data_extent[Index];
    }
    switch (SliceOrientation)
    {
    case SLICE_ORIENTATION_AXIAL:
        min_slice = m_data_extent[4];
        max_slice = m_data_extent[5];
        m_renderer = m_mpr->m_renderer;
        break;
    case SLICE_ORIENTATION_CORONAL:
        min_slice = m_data_extent[2];
        max_slice = m_data_extent[3];
        m_renderer = m_mpr->m_renderer2;
        break;
    case SLICE_ORIENTATION_SAGITTAL:
        min_slice = m_data_extent[0];
        max_slice = m_data_extent[1];
        m_renderer = m_mpr->m_renderer3;
        break;
    default:
        m_renderer = m_mpr->m_renderer ;
        ;
    }

    int adv = max_slice - min_slice + 1;
    m_cur_cross_normal = adv / 2;

    UpdateDisplay();
}

void CrossViewVtkActorBase::SetSlice(int delta_slice)
{
    int slice = m_cur_cross_normal + delta_slice;
    if (slice  < min_slice)
    {
        slice = min_slice;
    }
    if (slice > max_slice)
    {
        slice = max_slice;
    }
    
    m_cur_cross_normal  = slice;

    UpdateDisplay();

    //////////////////////////////////////////////////////////////////////////
    // Figure out the correct clipping range
    m_renderer->ResetCameraClippingRange();
    //////////////////////////////////////////////////////////////////////////

    printf("%s : slice %3d\n", m_actor_name.c_str(), m_cur_cross_normal);

    m_mpr->m_renderWindow->Render();

    return ;
}

void CrossViewVtkActorBase::UpdateDisplay()
{
    // 设置显示3D切层的位置
    switch (SliceOrientation)
    {
    case SLICE_ORIENTATION_AXIAL:
        this->SetDisplayExtent(m_data_extent[0], m_data_extent[1], m_data_extent[2], m_data_extent[3], m_cur_cross_normal, m_cur_cross_normal);
        break;
    case SLICE_ORIENTATION_CORONAL:
        this->SetDisplayExtent(m_data_extent[0], m_data_extent[1], m_cur_cross_normal, m_cur_cross_normal, m_data_extent[4], m_data_extent[5]);
        break;
    case SLICE_ORIENTATION_SAGITTAL:
        this->SetDisplayExtent(m_cur_cross_normal, m_cur_cross_normal, m_data_extent[2], m_data_extent[3], m_data_extent[4], m_data_extent[5]);
        break;
    default:
        ;
    }
}

/////////////////////////////////////////////////////////////////
vtkStandardNewMacro(CrossViewVtkActorAxial);

CrossViewVtkActorAxial::CrossViewVtkActorAxial()
{
    SliceOrientation = SLICE_ORIENTATION_AXIAL;
    m_actor_name = "Axial";
}

CrossViewVtkActorAxial::~CrossViewVtkActorAxial()
{
}
/////////////////////////////////////////////////////////
vtkStandardNewMacro(CrossViewVtkActorCoronal);

CrossViewVtkActorCoronal::CrossViewVtkActorCoronal()
{
    SliceOrientation = SLICE_ORIENTATION_CORONAL;
    m_actor_name = "Coronal";
}

CrossViewVtkActorCoronal::~CrossViewVtkActorCoronal()
{
}
/////////////////////////////////////////////////////////
vtkStandardNewMacro(CrossViewVtkActorSagittal);

CrossViewVtkActorSagittal::CrossViewVtkActorSagittal()
{
    SliceOrientation = SLICE_ORIENTATION_SAGITTAL;
    m_actor_name = "Sagittal";
}

CrossViewVtkActorSagittal::~CrossViewVtkActorSagittal()
{
}
/////////////////////////////////////////////////////////