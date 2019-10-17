/*
 *  
 *  $Id: LeftDownVtkActor.cpp $
 *  DeepWise LeftDownVtkActor
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
#include "left_down_vtk_actor.h"
//////////////////////////////////////////////////////////////////////////
class  LeftDownStatusMessage
{
public:
    static std::string Format(int slice, int maxSlice)
    {
        std::stringstream tmp;
        tmp << "Slice Number " << slice + 1 << "/" << maxSlice + 1;
        return tmp.str();
    }
    static std::string TransferString(int value)
    {
        std::stringstream tmp;
        tmp << value;
        return tmp.str();
    }
    static std::string TransferString(float value)
    {
        std::stringstream tmp;
        tmp << value;
        return tmp.str();
    }

};
//////////////////////////////////////////////////////////////////////////


vtkStandardNewMacro(LeftDownVtkActor);

LeftDownVtkActor::LeftDownVtkActor()
{
    vtkSmartPointer<vtkTextProperty> sliceTextProp = vtkSmartPointer<vtkTextProperty>::New();
    sliceTextProp->SetFontFamilyToCourier();
    sliceTextProp->SetFontSize(20);
    sliceTextProp->SetVerticalJustificationToBottom();
    sliceTextProp->SetJustificationToLeft();

    vtkSmartPointer<vtkTextMapper> sliceTextMapper = vtkSmartPointer<vtkTextMapper>::New();
    std::string msg = "";// LeftDownStatusMessage::Format(imageViewer->GetSliceMin(), imageViewer->GetSliceMax());
    sliceTextMapper->SetInput(msg.c_str());
    sliceTextMapper->SetTextProperty(sliceTextProp);

    this->SetMapper(sliceTextMapper);
    this->SetPosition(15, 10);
}

LeftDownVtkActor::~LeftDownVtkActor()
{
	
};


