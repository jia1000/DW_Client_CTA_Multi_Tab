/*=========================================================================

  Program:   ImagingEngine
  Module:    dicom_format_converter.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include <vector>
#include "data/vector2.h"

using namespace std;

/**
Class commissioned to validate and transform DICOM formats of Value Representation into internal formats
By defining the different Value Representation (VR) See PS 3.5-Table 6.2-1 DICOM VALUE REPRESENTATIONS
  */
class DICOMValueRepresentationConverter {
public:
    DICOMValueRepresentationConverter();
    ~DICOMValueRepresentationConverter();

    /// Statement of constant to define separator character by multi-valued DICOM attributes (Value Multiplicity (VM) > 1)
    static const string ValuesSeparator;
    
	/// Given a text string with decimal values (Decimal String), it turns it into a vector of doubles
	///If the string does not comply with DICOM's Decimal String definition, the vector will be empty and the value of the OK parameter will be false
	///	If the string is empty, the returned vector will be empty and OK will be true
	///	If the string is in the correct format, we will return a doubles vector with the corresponding values, and OK is true
    static vector<double> DecimalStringToDoubleVector(const string &decimal_string);

    /// Given a decimal string, converts it to a 2D vector. If the decimal has less or more than 2 decimal values it is considered invalid
    /// ok will be false and a 0 initialized vector will be returned. It's an specific method for decimal strings with two values
    static Vector2d DecimalStringTo2DDoubleVector(const string &decimal_string);
};
