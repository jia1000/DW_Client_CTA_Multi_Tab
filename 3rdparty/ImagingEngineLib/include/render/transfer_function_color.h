/*=========================================================================

  Program:   ImagingEngine
  Module:    transfer_function_color.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "vtk_include.h"
#include "data/data_definition.h"
#include "data/vector3.h"
#include "data/color4.h"
#include "render/transfer_function_template.h"

using namespace std;
using namespace DW::IMAGE;

namespace DW {
	namespace Render {

		/// Returns a default Color4 value.
		template <>
		Color4 TransferFunctionTemplate<Color4>::defaultValue();
		/// Returns the linear interpolation between A and B evaluating "A + Alpha * (b-a)", which is equivalent to "a * (1-alpha) + b * alpha".
		template <>
		Color4 TransferFunctionTemplate<Color4>::linearInterpolation(const Color4 &a, const Color4 &b, double alpha);

		/**
		It Represents a f: X-> C, where X is the set of property values (real) and C color Set (QColors).
		This function has a few explicitly defined points and the rest is obtained by linear interpolation or extrapolation of the closest neighbor.
		The transfer function also has a name.
		*/
		class ColorTransferFunction : public TransferFunctionTemplate<Color4>
		{
		public:
			/// Creates an empty transfer function, without any point and with no name.
			ColorTransferFunction();
			ColorTransferFunction(const ColorTransferFunction &func);
			~ColorTransferFunction();

			ColorTransferFunction& operator =(const ColorTransferFunction &func);

			/// Explicitly Defines the dot (x, color).
			/// You Must redefine it if the other variants are defined
			void Set(double x, const Color4 &color);
			/// Explicitly Defines the point (X, (red, green, blue)).
			void Set(double x, unsigned char red, unsigned char green, unsigned char blue);
			/// Explicitly Defines the point (X, (red, green, blue)).
			void Set(double x, float red, float green, float blue);

			/// Returns the color transfer function in VTK format.
			::vtkColorTransferFunction* GetVtkColorTransferFunction() const;

			/// Returns the function represented in the form of string.
			string ToString() const;

		private:
			/// Color4 transfer Function in VTK format.
			/// It must be saved in order to make the Delete () later
			mutable ::vtkColorTransferFunction *vtk_Color_func_;

		};

	}
}