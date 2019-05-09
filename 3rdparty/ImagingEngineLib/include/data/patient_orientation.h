/*=========================================================================

  Program:   ImagingEngine
  Module:    patient_orientation.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include <string>
#include "data/data_definition.h"
#include "data/image_orientation.h"
#include "data/vector3.h"

using namespace std;

namespace DW {
	namespace IMAGE {

		///
		// A class that encapsulate the attribute DICOM Patient Orientation (0020.0020). For more information see section C. 7.6.1.1.1 (PS 3.3)
		// If this attribute is directly assigned to the contents of the DICOM file it will consist of two separate text strings separated by \ \
		// That represent the direction of the rows and columns of the image with respect to the patient.
		// In case the attribute is calculated from the ImageOrientation class, it also added a third string that would indicate
		// The direction in which the images are stacker, useful for reconstructions.
		///
		class PatientOrientation
		{
		public:
			/// Orientation Labels
			/// If Anatomical Orientation Type (0010.2210) is empty or has the value of BIPED,
			/// These will be the abbreviations designated by the anatomical directorates
			/// If Anatomical Orientation Type (0010.2210) has QUADRUPED, it would be used other abbreviations (animals)
			/// TODO at the moment we only consider the "BIPED" type orientations. Consult C. 7.6.1.1.1 (PS 3.3) For more information
			static const string kLeftLabel;
			static const string kRightLabel;
			static const string kPosteriorLabel;
			static const string kAnteriorLabel;
			static const string kHeadLabel;
			static const string kFeetLabel;

			/// Assigns the orientation labels for each direction: row, column, normal. The label corresponding to the normal is optional.
			/// In case the tags provided are not valid, it will return false, true otherwise.
			bool setLabels(const string &rowDirectionLabel, const string &columnDirectionLabel, const string &normalDirectionLabel = string());

			/// Assigns the orientation string in DICOM format. If this has any inconsistency, it will return false, true otherwise.
			/// Empty strings are accepted, with 2 and 3 separate items per \ \ and containing the orientation labels stipulated by DICOM
			bool setDICOMFormattedPatientOrientation(const string &patientOrientation);

			/// Return of the chain of patient orientation in DICOM format
			string getDICOMFormattedPatientOrientation() const;

			/// Assigns patient orientation calculated from ImageOrientation
			void setPatientOrientationFromImageOrientation(const ImageOrientation &imageOrientation);

			/// It returns the orientation tag of the direction of the row, column or normal, if it exists.
			string getRowDirectionLabel() const;
			string getColumnDirectionLabel() const;
			string getNormalDirectionLabel() const;

			/// Given an orientation tag, this label returns us but with the opposite values.
			/// For example, if the label that they give us is RPF (Right-back, Feet), the return value would be LAH (Left-Previous-Head)
			/// Valid labels and their opposites are as follows:
			/// RightLabel: LeftLabel, AnteriorLabel: PosteriorLabel, HeadLabel: FeetLabel
			/// If the given string does not correspond with any of these values, will the transformed value be '? '
			static string getOppositeOrientationLabel(const string &label);

			/// Equality operator
			bool operator==(const PatientOrientation &orientation) const;

		private:
			/// We validate a chain of patient orientation in DICOM format. We will accept chains with 2 or 3 elements or empty.
			/// If the string is correct, it returns true, False otherwise
			bool validateDICOMFormattedPatientOrientationString(const string &string);

			/// Given a direction vector returns the corresponding orientation tag
			string getOrientationLabelFromDirectionVector(const Vector3d &vector);

			/// Returns the Nth orientation tag (row = 0, column = 1, Normal = 2).
			/// If we give an out of range index for the number of labels there, return an empty string.
			string getNthDirectionLabel(int i) const;

		private:
			/// The chain of patient orientation. It will be saved as in the DICOM format, admitting it is empty or contains 2 or 3 elements.
			string patient_orientation_;
		};

	}
}
