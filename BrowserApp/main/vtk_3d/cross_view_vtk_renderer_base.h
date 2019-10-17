/*
 *  
 *  $Id: CrossViewVtkRenderer.h $
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


class CrossViewVtkRendererBase : public vtkRenderer {
public:
	static CrossViewVtkRendererBase *New();

    void SetSlice(int delta_slice);

    // 重写基类vtkRenderer的纯虚----begin-----------------
    virtual double GetPickedZ();

    virtual void DevicePickRender();

    virtual void StartPick(unsigned int pickFromSize);

    virtual void UpdatePickId();

    virtual void DonePick();

    virtual unsigned int GetPickedId();

    virtual unsigned int GetNumPickedIds();

    virtual int GetPickedIds(unsigned int atMost, unsigned int* callerBuffer);

    virtual void DeviceRender();
    // 重写基类vtkRenderer的纯虚----end---------------------

protected:
    CrossViewVtkRendererBase();
	~CrossViewVtkRendererBase();

	vtkTypeMacro(CrossViewVtkRendererBase, vtkRenderer);
	
private:
    vtkSmartPointer<vtkDICOMImageReader> m_v16;

    WidgetsMprVtk* m_mpr;
};
