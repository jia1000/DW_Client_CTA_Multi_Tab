/*=========================================================================

  Program:   ImagingEngine
  Module:    transfer_function.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "transfer_function.h"

using namespace DW::Render;

TransferFunction::TransferFunction() 
	: modified_(true)
{

}

TransferFunction::TransferFunction(vtkLookupTable *lookupTable)
	: modified_(true)
{
	// We Transform the vtkLookupTable into our format
	double range[2];
	lookupTable->GetTableRange(range);

	// Let's calculate the difference between value and value of the table
	double step = (range[1] - range[0]) / lookupTable->GetNumberOfTableValues();

	// We travel the vtkLookupTable and we insert the values in our format
	for (double value = range[0]; value < range[1]; value += step)
	{
		double rgb[3];
		double opacity;
		lookupTable->GetColor(value, rgb);
		opacity = lookupTable->GetOpacity(value);
		color_func_.set(value, Color4((float)rgb[0], (float)rgb[1], (float)rgb[2]));
		opacity_func_.set(value, opacity);
	}
}

const string& TransferFunction::GetName() const
{
	return color_func_.name();
}

void TransferFunction::SetName(const string &name)
{
	color_func_.setName(name);
	opacity_func_.setName(name);
	gradient_func_.setName(name);
}

bool TransferFunction::operator ==(const TransferFunction &transferFunction) const
{
	//TODO: TransferFunctionTemplate<Color4>重载==操作符有问题
	//return color_func_ == transferFunction.color_func_ && 
	//	opacity_func_ == transferFunction.opacity_func_ &&
	//	gradient_func_ == transferFunction.gradient_func_;
	return true;
}

vtkColorTransferFunction* TransferFunction::vtkColorTransferFunction() const
{
	return color_func_.GetVtkColorTransferFunction();
}

vtkPiecewiseFunction* TransferFunction::vtkScalarOpacityTransferFunction() const
{
	return opacity_func_.GetVtkOpacityTransferFunction();
}

vtkPiecewiseFunction* TransferFunction::vtkGradientOpacityTransferFunction() const
{
	return gradient_func_.GetVtkOpacityTransferFunction();
}

//ostream& operator <<(ostream &stream, const TransferFunction &transferFunction)
//{
//	return stream << transferFunction.color_func_ << transferFunction.opacity_func_ << transferFunction.gradient_func_;
//}
//
//ostream& operator >>(ostream &stream, TransferFunction &transferFunction)
//{
//	return stream >> transferFunction.color_func_ >> transferFunction.opacity_func_ >> transferFunction.gradient_func_;
//}

void TransferFunction::SetColor(double x, unsigned char red, unsigned char green, unsigned char blue)
{
	color_func_.Set(x, red, green, blue);
	modified_ = true;
}

void TransferFunction::SetColor(double x, float red, float green, float blue)
{
	color_func_.Set(x, red, green, blue);
	modified_ = true;
}

void TransferFunction::SetScalarOpacity(double x, double opacity)
{
	opacity_func_.set(x, opacity);
	modified_ = true;
}

void TransferFunction::SetGradientOpacity(double y, double opacity)
{
	gradient_func_.set(y, opacity);
	modified_ = true;
}
