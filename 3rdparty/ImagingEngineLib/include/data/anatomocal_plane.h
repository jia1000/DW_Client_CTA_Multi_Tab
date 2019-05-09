/*=========================================================================

  Program:   ImagingEngine
  Module:    anotomocal_plane.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "data/vector3.h"
#include "data/patient_orientation.h"

using namespace std;

namespace DW {
	namespace IMAGE {
				
		///
		// The AnatomicalPlane class defines the anatomical orientations of a patient.
		///
		class AnatomicalPlane {
		public:
			/// Anatomical orientation types.
			enum Plane { Axial, Sagittal, Coronal, Oblique, NotAvailable };

			AnatomicalPlane(Plane plane = Axial);

			/// Returns the label corresponding to this anatomical plane to display it in the UI.
			string GetLabel() const;

			/// Returns the label corresponding to the anatomical plane corresponding to the given patient orientation.
			static string GetLabelFromPatientOrientation(const PatientOrientation &orientation);

			/// Returns the anatomical plane corresponding to the given patient orientation.
			static AnatomicalPlane GetPlaneFromPatientOrientation(const PatientOrientation &orientation);

			/// Returns the default patient orientation for this anatomical plane.
			PatientOrientation GetDefaultRadiologicalOrienation() const;

			/// Conversion operator: allows to use an AnatomicalPlane in a switch.
			operator Plane() const;

		private:
			Plane plane_;

		};

	}
}