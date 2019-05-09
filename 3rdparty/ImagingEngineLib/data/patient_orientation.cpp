/*=========================================================================

  Program:   ImagingEngine
  Module:    patient_orientation.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "data/patient_orientation.h"
#include "tools/dicom_format_converter.h"

using namespace DW::IMAGE;

const string PatientOrientation::kLeftLabel("L");
const string PatientOrientation::kRightLabel("R");
const string PatientOrientation::kPosteriorLabel("P");
const string PatientOrientation::kAnteriorLabel("A");
const string PatientOrientation::kHeadLabel("H");
const string PatientOrientation::kFeetLabel("F");

bool PatientOrientation::setLabels(const string &rowDirectionLabel, const string &columnDirectionLabel, const string &normalDirectionLabel)
{
	string dicomFormattedString = rowDirectionLabel + DICOMValueRepresentationConverter::ValuesSeparator + columnDirectionLabel;

	if (!normalDirectionLabel.empty())
	{
		dicomFormattedString += DICOMValueRepresentationConverter::ValuesSeparator + normalDirectionLabel;
	}

	return setDICOMFormattedPatientOrientation(dicomFormattedString);
}

bool PatientOrientation::setDICOMFormattedPatientOrientation(const string &patientOrientation)
{
	if (validateDICOMFormattedPatientOrientationString(patientOrientation))
	{
		patient_orientation_ = patientOrientation;
		return true;
	}
	else
	{
		patient_orientation_ = "";
		return false;
	}
}

string PatientOrientation::getDICOMFormattedPatientOrientation() const
{
	return patient_orientation_;
}

void PatientOrientation::setPatientOrientationFromImageOrientation(const ImageOrientation &imageOrientation)
{
	string rowLabel = this->getOrientationLabelFromDirectionVector(imageOrientation.GetRowVector());
	string columnLabel = this->getOrientationLabelFromDirectionVector(imageOrientation.GetColumnVector());
	string normalLabel = this->getOrientationLabelFromDirectionVector(imageOrientation.GetNormalVector());

	if (!setLabels(rowLabel, columnLabel, normalLabel))
	{
		//DEBUG_LOG("makePatientOrientationFromImageOrientationPatient() Generated a chain of invalid patient orientation");
	}
}

string PatientOrientation::getRowDirectionLabel() const
{
	return getNthDirectionLabel(0);
}

string PatientOrientation::getColumnDirectionLabel() const
{
	return getNthDirectionLabel(1);
}

string PatientOrientation::getNormalDirectionLabel() const
{
	return getNthDirectionLabel(2);
}

string PatientOrientation::getNthDirectionLabel(int i) const
{
	string label;

	if (!patient_orientation_.empty())
	{
// 		string[] labelList = patient_orientation_.split(DICOMValueRepresentationConverter::ValuesSeparator);
// 		if (labelList.size() >= i + 1 && i >= 0)
// 		{
// 			label = labelList.at(i);
// 		}
	}

	return label;
}

string PatientOrientation::getOppositeOrientationLabel(const string &label)
{
	int i = 0;
	string oppositeLabel;
	while (i < label.size())
	{
		if (label.at(i) == PatientOrientation::kLeftLabel[0])
		{
			oppositeLabel += PatientOrientation::kRightLabel;
		}
		else if (label.at(i) == PatientOrientation::kRightLabel[0])
		{
			oppositeLabel += PatientOrientation::kLeftLabel;
		}
		else if (label.at(i) == PatientOrientation::kAnteriorLabel[0])
		{
			oppositeLabel += PatientOrientation::kPosteriorLabel;
		}
		else if (label.at(i) == PatientOrientation::kPosteriorLabel[0])
		{
			oppositeLabel += PatientOrientation::kAnteriorLabel;
		}
		else if (label.at(i) == PatientOrientation::kHeadLabel[0])
		{
			oppositeLabel += PatientOrientation::kFeetLabel;
		}
		else if (label.at(i) == PatientOrientation::kFeetLabel[0])
		{
			oppositeLabel += PatientOrientation::kHeadLabel;
		}
		else
		{
			oppositeLabel += "?";
		}
		i++;
	}
	return oppositeLabel;
}

bool PatientOrientation::operator==(const PatientOrientation &orientation) const
{
	return patient_orientation_ == orientation.patient_orientation_;
}

bool PatientOrientation::validateDICOMFormattedPatientOrientationString(const string &string)
{
	//// We construct the regular expression that checks us that the string is in the correct format: empty string, or with 2 or 3 elements.
	//// Valid Tags: RLAPHF
	//string validOrientationLabels = kRightLabel + kLeftLabel + kAnteriorLabel + kPosteriorLabel + kHeadLabel + kFeetLabel;

	//// Expression for [RLAPHF] + tags
	//string validLabelsExpression = "[" + validOrientationLabels + "]+";

	//// Expression by separators
	//// We have to put two times followed DICOMValueRepresentationConverter:: ValuesSeparator, because as it says the documentation of QRegExp:
	////
	//// Note: The C++ compiler transforms backslashes in strings. To include a \ in to RegExp, enter it twice, i.e. \ \. 
	//// To match the backslash character itself, enter it four times, i.e. \ \ \ \.
	//// 
	//// Then you must put \ \ \ \ To make the expression correct
	//string separatorExpression = DICOMValueRepresentationConverter::ValuesSeparator + DICOMValueRepresentationConverter::ValuesSeparator;

	//// Final expression: ([RLAPHF] + \ \ \ \ [RLAPHF] + (\ \ \ \ [RLAPHF] +)?) *
	//// That is, 2 or 3 items with valid labels separated by \ \.
	//string regExpString = "(" + validLabelsExpression + separatorExpression + validLabelsExpression +
	//	"(" + separatorExpression + validLabelsExpression + ")?)*";

	//QRegExp validStringExpression(regExpString);
	//return validStringExpression.exactMatch(string);

	return false;
}

string PatientOrientation::getOrientationLabelFromDirectionVector(const Vector3d &vector)
{
	string orientation;

	string orientationX = vector[0] < 0 ? kRightLabel : kLeftLabel;
	string orientationY = vector[1] < 0 ? kAnteriorLabel : kPosteriorLabel;
	string orientationZ = vector[2] < 0 ? kFeetLabel : kHeadLabel;

	double absX = fabs(vector[0]);
	double absY = fabs(vector[1]);
	double absZ = fabs(vector[2]);

	for (int i = 0; i < 3; ++i)
	{
		if (absX > .0001 && absX > absY && absX > absZ)
		{
			orientation += orientationX;
			absX = 0;
		}
		else if (absY > .0001 && absY > absX && absY > absZ)
		{
			orientation += orientationY;
			absY = 0;
		}
		else if (absZ > .0001 && absZ > absX && absZ > absY)
		{
			orientation += orientationZ;
			absZ = 0;
		}
		else
		{
			break;
		}
	}

	return orientation;
}