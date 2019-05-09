/*=========================================================================

  Program:   ImagingEngine
  Module:    transfer_function_color.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "render/transfer_function_color.h"

using namespace DW::Render;

template <>
Color4 TransferFunctionTemplate<Color4>::defaultValue()
{
	unsigned char a = 0;
	return Color4(a, a, a);
}

template <>
Color4 TransferFunctionTemplate<Color4>::linearInterpolation(const Color4 &a, const Color4 &b, double alpha)
{
	double red = a.RF() + alpha * (b.RF() - a.RF());
	double green = a.GF() + alpha * (b.GF() - a.GF());
	double blue = a.BF() + alpha * (b.BF() - a.BF());
	return Color4((float)red, (float)green, (float)blue);
}


ColorTransferFunction::ColorTransferFunction()
	: vtk_Color_func_(0)
{
}

ColorTransferFunction::ColorTransferFunction(const ColorTransferFunction &colorTransferFunction)
	: TransferFunctionTemplate<Color4>(colorTransferFunction), vtk_Color_func_(0)
{
}

ColorTransferFunction::~ColorTransferFunction()
{
	if (vtk_Color_func_)
	{
		vtk_Color_func_->Delete();
	}
}

ColorTransferFunction& ColorTransferFunction::operator =(const ColorTransferFunction &colorTransferFunction)
{
	if (this != &colorTransferFunction)
	{
		TransferFunctionTemplate<Color4>::operator =(colorTransferFunction);
		vtk_Color_func_ = 0;
	}

	return *this;
}

void ColorTransferFunction::Set(double x, const Color4 &color)
{
	TransferFunctionTemplate<Color4>::set(x, color);
}

void ColorTransferFunction::Set(double x, unsigned char red, unsigned char green, unsigned char blue)
{
	Set(x, Color4(red, green, blue));
}

void ColorTransferFunction::Set(double x, float red, float green, float blue)
{
	Set(x, Color4(red, green, blue));
}

vtkColorTransferFunction* ColorTransferFunction::GetVtkColorTransferFunction() const
{
	if (vtk_Color_func_)
	{
		vtk_Color_func_->Delete();
	}
	vtk_Color_func_ = vtkColorTransferFunction::New();

	auto it = m_map.begin();
	while(it != m_map.end())
	{
		vtk_Color_func_->AddRGBPoint(it->first, it->second.RF(), it->second.GF(), it->second.BF());
		it ++;         
	}

	return vtk_Color_func_;
}

string ColorTransferFunction::ToString() const
{
	string str;
	return str;
}
