/*=========================================================================

  Program:   ImagingEngine
  Module:    image_orientation.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "data/image_orientation.h"
#include "tools/math.h"
#include "tools/string_util.h"
#include "tools/dicom_format_converter.h"

using namespace DW::IMAGE;

/// Constant to define what we consider an empty vector
const Vector3d EmptyVector(.0,.0,.0);

ImageOrientation::ImageOrientation(const string& orientation)
{
	SetVectorValuesToDefault();
}

ImageOrientation::ImageOrientation(const Vector3d& row_vector, const Vector3d& column_vector)
{
	SetRowAndColumnVectors(row_vector, column_vector);
}

ImageOrientation::~ImageOrientation()
{

}

Vector3d ImageOrientation::GetRowVector() const
{
	return row_vector_;
}

Vector3d ImageOrientation::GetColumnVector() const
{
	return column_vector_;
}

Vector3d ImageOrientation::GetNormalVector() const
{
	return normal_vector_;
}

bool ImageOrientation::SetImageOrientationString(const string &image_orientation)
{
	if (image_orientation.empty())
	{
		SetVectorValuesToDefault();
		return true;
	}

	// From here we check that there is the correct number of items
	vector<string> list = Split(image_orientation, DICOMValueRepresentationConverter::ValuesSeparator);
	if (list.size() != 6)
	{
		/// Invalid dicom format
		return false;
	}

	/// We have 6 items, you should check that are valid values
	vector<double> converted_values = DICOMValueRepresentationConverter::DecimalStringToDoubleVector(image_orientation);

	// At this point, we have 6 values and are valid
	// We Assign the converted values to the corresponding vectors
	Vector3d row(converted_values[0], converted_values[1], converted_values[2]);
	Vector3d column(converted_values[3], converted_values[4], converted_values[5]);
	SetRowAndColumnVectors(row, column);

	return true;
}

string ImageOrientation::GetImageOrientationAsString() const
{
	if (row_vector_ == EmptyVector && column_vector_ == EmptyVector)
	{
		return string();
	}
	else
	{
		string image_orientation;

		image_orientation = to_string(row_vector_.x) + DICOMValueRepresentationConverter::ValuesSeparator;
		image_orientation += to_string(row_vector_.y) + DICOMValueRepresentationConverter::ValuesSeparator;
		image_orientation += to_string(row_vector_.z) + DICOMValueRepresentationConverter::ValuesSeparator;

		image_orientation += to_string(column_vector_.x) + DICOMValueRepresentationConverter::ValuesSeparator;
		image_orientation += to_string(column_vector_.y) + DICOMValueRepresentationConverter::ValuesSeparator;
		image_orientation += to_string(column_vector_.z);

		return image_orientation;
	}
}

void ImageOrientation::SetRowAndColumnVectors(const Vector3d &row_vector, const Vector3d &column_vector)
{
	row_vector_ = row_vector;
	column_vector_ = column_vector;
	MathTool::Cross(row_vector, column_vector, normal_vector_);
}

bool ImageOrientation::operator==(const ImageOrientation& image_orientation) const
{
	return (row_vector_ == image_orientation.row_vector_) 
		&& (column_vector_ == image_orientation.column_vector_);
}