/*=========================================================================

  Program:   ImagingEngine
  Module:    transfer_function_opacity.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "render/transfer_function_opacity.h"

using namespace DW::Render;

OpacityTransferFunction::OpacityTransferFunction()
	: vtk_opacity_func_(0)
{
}

OpacityTransferFunction::OpacityTransferFunction(const OpacityTransferFunction &opacityTransferFunction)
	: TransferFunctionTemplate<double>(opacityTransferFunction), vtk_opacity_func_(0)
{
}

OpacityTransferFunction::~OpacityTransferFunction()
{
	if (vtk_opacity_func_)
	{
		vtk_opacity_func_->Delete();
	}
}

OpacityTransferFunction& OpacityTransferFunction::operator =(const OpacityTransferFunction &opacityTransferFunction)
{
	if (this != &opacityTransferFunction)
	{
		TransferFunctionTemplate<double>::operator =(opacityTransferFunction);
		vtk_opacity_func_ = 0;
	}

	return *this;
}

vtkPiecewiseFunction* OpacityTransferFunction::GetVtkOpacityTransferFunction() const
{
	if (vtk_opacity_func_)
	{
		vtk_opacity_func_->Delete();
	}
	vtk_opacity_func_ = vtkPiecewiseFunction::New();

	auto it = m_map.begin();
	while(it != m_map.end())
	{
		vtk_opacity_func_->AddPoint(it->first, it->second);
		it ++;         
	}
	
	return vtk_opacity_func_;
}