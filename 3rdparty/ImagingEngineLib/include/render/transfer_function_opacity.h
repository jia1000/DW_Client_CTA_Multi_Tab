/*=========================================================================

  Program:   ImagingEngine
  Module:    transfer_function_opacity.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "vtk_include.h"
#include "data/data_definition.h"
#include "data/vector3.h"
#include "render/transfer_function_template.h"

using namespace std;

namespace DW {
	namespace Render {

		/**
		Represents a function of opacity F: X-> Or, where X is a set of actuals (property values or magnitude of gradient) and
		Set of opacities (real in the range [0.1]).
		This function has a few explicitly defined points and the rest is obtained by linear interpolation or extrapolation of the closest neighbor.
		The transfer function also has a name.
		*/
		class OpacityTransferFunction : public TransferFunctionTemplate<double>
		{
		public:
			/// Creates an empty transfer function, without any point and with no name.
			OpacityTransferFunction();
			OpacityTransferFunction(const OpacityTransferFunction &opacityTransferFunction);
			~OpacityTransferFunction();

			OpacityTransferFunction& operator =(const OpacityTransferFunction &opacityTransferFunction);

			/// Returns the opacity transfer function in VTK format.
			vtkPiecewiseFunction* GetVtkOpacityTransferFunction() const;

		private:
			/// Opacity transfer Function in VTK format.
			/// It must be saved in order to make the Delete () later
			mutable vtkPiecewiseFunction *vtk_opacity_func_;
		};

	}
}
