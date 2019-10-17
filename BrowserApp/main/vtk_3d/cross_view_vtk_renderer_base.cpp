/*
 *  
 *  $Id: CrossViewVtkRenderer.cpp $
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
#include "cross_view_vtk_renderer_base.h"

#include "widgets_mpr_vtk.h"

vtkStandardNewMacro(CrossViewVtkRendererBase);

CrossViewVtkRendererBase::CrossViewVtkRendererBase()
{
   
}


CrossViewVtkRendererBase::~CrossViewVtkRendererBase()
{	
}


void CrossViewVtkRendererBase::SetSlice(int delta_slice)
{
    

    return ;
}


double CrossViewVtkRendererBase::GetPickedZ()
{
    return 0.0;
}
void CrossViewVtkRendererBase::DevicePickRender()
{

}
void CrossViewVtkRendererBase::StartPick(unsigned int pickFromSize)
{

}

void CrossViewVtkRendererBase::UpdatePickId()
{

}

void CrossViewVtkRendererBase::DonePick()
{

}

unsigned int CrossViewVtkRendererBase::GetPickedId()
{
    return 0;
}

unsigned int CrossViewVtkRendererBase::GetNumPickedIds()
{
    return 0;
}

int CrossViewVtkRendererBase::GetPickedIds(unsigned int atMost, unsigned int* callerBuffer)
{
    return 0;
}

void CrossViewVtkRendererBase::DeviceRender()
{

}