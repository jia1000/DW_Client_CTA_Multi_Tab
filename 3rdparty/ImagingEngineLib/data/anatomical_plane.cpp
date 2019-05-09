/*=========================================================================

  Program:   ImagingEngine
  Module:    anatomical_plane.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "anatomocal_plane.h"

using namespace DW::IMAGE;


AnatomicalPlane::AnatomicalPlane(Plane plane)
	: plane_(plane)
{
}

string AnatomicalPlane::GetLabel() const
{
	switch (plane_)
	{
	case Axial:
		return string("AXIAL");

	case Sagittal:
		return string("SAGITTAL");

	case Coronal:
		return string("CORONAL");

	case Oblique:
		return string("OBLIQUE");

	case NotAvailable:
		return string("N/A");

	default:
		return string();
	}
}

string AnatomicalPlane::GetLabelFromPatientOrientation(const PatientOrientation &orientation)
{
	return GetPlaneFromPatientOrientation(orientation).GetLabel();
}

AnatomicalPlane AnatomicalPlane::GetPlaneFromPatientOrientation(const PatientOrientation &orientation)
{
	string rowAxis = orientation.getRowDirectionLabel();
	string columnAxis = orientation.getColumnDirectionLabel();

	if (!rowAxis.empty() && !columnAxis.empty())
	{
		//if ((rowAxis.startsWith(PatientOrientation::kRightLabel) || rowAxis.startsWith(PatientOrientation::kLeftLabel)) &&
		//	(columnAxis.startsWith(PatientOrientation::kAnteriorLabel) || columnAxis.startsWith(PatientOrientation::kPosteriorLabel)))
		//{
		//	return Axial;
		//}
		//else if ((columnAxis.startsWith(PatientOrientation::kRightLabel) || columnAxis.startsWith(PatientOrientation::kLeftLabel)) &&
		//	(rowAxis.startsWith(PatientOrientation::kAnteriorLabel) || rowAxis.startsWith(PatientOrientation::kPosteriorLabel)))
		//{
		//	return Axial;
		//}
		//else if ((rowAxis.startsWith(PatientOrientation::kRightLabel) || rowAxis.startsWith(PatientOrientation::kLeftLabel)) &&
		//	(columnAxis.startsWith(PatientOrientation::kHeadLabel) || columnAxis.startsWith(PatientOrientation::kFeetLabel)))
		//{
		//	return Coronal;
		//}
		//else if ((columnAxis.startsWith(PatientOrientation::kRightLabel) || columnAxis.startsWith(PatientOrientation::kLeftLabel)) &&
		//	(rowAxis.startsWith(PatientOrientation::kHeadLabel) || rowAxis.startsWith(PatientOrientation::kFeetLabel)))
		//{
		//	return Coronal;
		//}
		//else if ((rowAxis.startsWith(PatientOrientation::kAnteriorLabel) || rowAxis.startsWith(PatientOrientation::kPosteriorLabel)) &&
		//	(columnAxis.startsWith(PatientOrientation::kHeadLabel) || columnAxis.startsWith(PatientOrientation::kFeetLabel)))
		//{
		//	return Sagittal;
		//}
		//else if ((columnAxis.startsWith(PatientOrientation::kAnteriorLabel) || columnAxis.startsWith(PatientOrientation::kPosteriorLabel)) &&
		//	(rowAxis.startsWith(PatientOrientation::kHeadLabel) || rowAxis.startsWith(PatientOrientation::kFeetLabel)))
		//{
		//	return Sagittal;
		//}
		//else
		//{
		//	return Oblique;
		//}

		return Oblique;
	}
	else
	{
		return NotAvailable;
	}
}

PatientOrientation AnatomicalPlane::GetDefaultRadiologicalOrienation() const
{
	PatientOrientation orientation;

	switch (plane_)
	{
	case Axial:
		orientation.setLabels(PatientOrientation::kLeftLabel, PatientOrientation::kPosteriorLabel);
		break;

	case Sagittal:
		orientation.setLabels(PatientOrientation::kPosteriorLabel, PatientOrientation::kFeetLabel);
		break;

	case Coronal:
		orientation.setLabels(PatientOrientation::kLeftLabel, PatientOrientation::kFeetLabel);
		break;

	case Oblique:
	case NotAvailable:
	default:
		// We don't have enough information to provide such information
		break;
	}

	return orientation;
}

AnatomicalPlane::operator Plane() const
{
	return plane_;
}