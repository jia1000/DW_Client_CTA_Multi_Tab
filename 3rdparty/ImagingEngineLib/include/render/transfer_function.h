/*=========================================================================

  Program:   ImagingEngine
  Module:    transfer_function.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "vtk_include.h"
#include "data/data_definition.h"
#include "data/vector3.h"
#include <vtkLookupTable.h>
#include "data/color4.h"
#include "render/transfer_function_color.h"
#include "render/transfer_function_opacity.h"

using namespace std;
using namespace DW::IMAGE;

namespace DW {
	namespace Render {
		
		/// It Represents a transfer function f: (X, Y)-> (C, O), where X is the set of property values (real), Y the set of magnitudes of the gradient
		/// (real positive), C the set of colors (QColors) and O the set of opacities (real in the range [0.1]).
		/// Actually it consists of a transfer function FC: X-> C, an opacity scalar fo: X-> O, and opacity of the gradient FG: Y-> O.
		/// There are explicitly defined points and the rest are obtained by linear interpolation or extrapolation of the closest neighbor.
		/// You can work with the colour, scalar opacity and the opacity of the gradient separately (recommended option) or together. The transfer function
		/// also has a name.
		class TransferFunction 
		{
		public:
			/// Build an empty transfer function, no point and no name.
			TransferFunction();
			/// Builds a transfer function from a vtkLookupTable object.
			TransferFunction(vtkLookupTable *lookupTable);

			bool operator ==(const TransferFunction &transferFunction) const;

			/// Returns the name.
			const string& GetName() const;
			/// Assign name.
			void SetName(const string &name);

			/*----------------Methods for editting transfer function-------------------
			///// Returns the pair of color and opacity corresponding to the property value x. It Does not consider the gradient opacity.
			///// \note The opacity is encapsulated in the QColor alpha channel, thus losing accuracy. To avoid this, it is recommended to access
			///// 	To color and opacity separately.
			//Color4 Get(double x) const;
			///// Returns the corresponding color to the property value x.
			//Color4 GetColor(double x) const;
			///// Returns the scalar opacity corresponding to the property value x.
			///// Equivalent to getScalarOpacity (x).
			//double GetOpacity(double x) const;
			///// Returns the scalar opacity corresponding to the property value x.
			//double GetScalarOpacity(double x) const;
			///// Returns the opacity of the gradient for the magnitude of gradient and.
			//double GetGradientOpacity(double y) const;
			---------------------End--------------------*/
			
			/// Returns the color transfer function in VTK format.
			vtkColorTransferFunction* vtkColorTransferFunction() const;
			/// Returns The scale opacity transfer function in VTK format.
			vtkPiecewiseFunction* vtkScalarOpacityTransferFunction() const;
			/// Returns the opacity gradient transfer function in VTK format.
			vtkPiecewiseFunction* vtkGradientOpacityTransferFunction() const;

			///// Writes the given transfer function to the given stream.
			//friend ostream& operator <<(ostream &stream, const TransferFunction &transferFunction);
			///// Fills the given transfer function from the given stream.
			//friend ostream& operator >>(ostream &stream, TransferFunction &transferFunction);

			/// Explicitly Defines the color point (x, (red, green, blue).
			void SetColor(double x, unsigned char red, unsigned char green, unsigned char blue);
			void SetColor(double x, float red, float green, float blue);
			/// Explicitly Defines the scalar opacity point (x, opacity).
			void SetScalarOpacity(double x, double opacity);
			/// It explicitly Defines the gradient point opacity (y, opacity).
			void SetGradientOpacity(double y, double opacity);


		private:
			/// A flag that indicates whether or not it has been modified
			bool modified_;
			/// Color4 Transfer Function.
			ColorTransferFunction color_func_;
			/// Scalar opacity transfer Function.
			OpacityTransferFunction opacity_func_;
			/// Gradient opacity transfer Function.
			OpacityTransferFunction gradient_func_;
			
		};

	}
}