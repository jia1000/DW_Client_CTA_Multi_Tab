/*=========================================================================

  Program:   ImagingEngine
  Module:    dicom_format_converter.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "tools/dicom_format_converter.h"
#include "tools/string_util.h"

/// Defining Constants
const string DICOMValueRepresentationConverter::ValuesSeparator("\\");


DICOMValueRepresentationConverter::DICOMValueRepresentationConverter()
{
}

DICOMValueRepresentationConverter::~DICOMValueRepresentationConverter()
{
}

vector<double> DICOMValueRepresentationConverter::DecimalStringToDoubleVector(const string &decimal_string)
{
	/// TODO Look if DCMTK or GDCM incorporate some class that does the strict verification of the values according to DICOM
	/// DCMTK class DcmDecimalString was found, but it is not clear that it can serve for this purpose
	/// For now we will accept as good the conversion of the decimal values that you give us with QLocale:: C
	/// By defining Decimal String (DS) See PS 3.5 ¨C Table 6.2 -1 DICOM VALUE REPRESENTATIONS
	vector<double> result;
	vector<string> values_list = Split(decimal_string, ValuesSeparator);

	// We Create a localOk to control the validation internally
	bool local_Ok;

	if (decimal_string.empty())
	{
		/// If the string is empty, we will return true and the returned vector is empty
		local_Ok = true;
	}
	else
	{
		auto it = values_list.begin();
		while (it != values_list.end()){
			double val = StringToNumber<double>(*it);
			result.push_back(val);
			it++;
		}
	}
	return result;
}

Vector2d DICOMValueRepresentationConverter::DecimalStringTo2DDoubleVector(const string &decimal_string)
{
	vector<double> vector_value = DICOMValueRepresentationConverter::DecimalStringToDoubleVector(decimal_string);

	Vector2d vector_2d_Value(0.0, 0.0);
	
	// Check if it's a real 2D vector
	if (vector_value.size() == 2)
	{
		vector_2d_Value[0] = vector_value[0];
		vector_2d_Value[1] = vector_value[1];
	}
	return vector_2d_Value;
}