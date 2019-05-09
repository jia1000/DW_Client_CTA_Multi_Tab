/*=========================================================================

  Program:   ImagingEngine
  Module:    image_overlay.h
  author: 	 zhangjian
  Brief:	 Class that encaptures the Overlay object of DICOM.
			 For more information, see section C. 9 OVERLAYS, PS 3.3.
			 
			 copy from ImageOverlay.h in starview project

=========================================================================*/
#pragma once

#include "data/data_definition.h"

namespace DW {
	namespace IMAGE {

		class ImageOverlay
		{
		public:
			ImageOverlay();
			~ImageOverlay();

			/// Assigns/fetches rows and columns from the overlay
			void SetRows(int rows);
			void SetColumns(int columns);
			int GetRows() const;
			int GetColumns() const;

			/// Assign/Get Overlay source
			void SetOrigin(int x, int y);
			int GetXOrigin() const;
			int GetYOrigin() const;

			/// Assign/Return overlay data
			void SetData(unsigned char *data);
			unsigned char* GetData() const;

			/// Returns certain SII The overlay is valid (if the number of rows and the number of columns are positive and has data).
			bool isValid() const;

		private:
			/// Overlay Rows and columns
			int m_rows, m_columns;

			/// Location of the first point of overlay with respect to the pixels in the image, given as Fila\columna.
			/// The top left Pixel has the coordinate 1 \ 1
			/// Column Values greater than 1 indicate that the source of the overlay plan is to the right of the image source.
			/// Row Values greater than 1 indicate that the source of the overlay plan is below the image source.
			/// Values below 1 indicate that the origin of the overlay plan is above or to the left of the image source.
			int m_origin[2];

			/// Overlay Data
			unsigned char* m_data;
		};

	}
}
