/*=========================================================================

  Program:   ImagingEngine
  Module:    pixel_spacing_2d.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "tools/math.h"
#include  <iostream>
#include <iomanip>

using namespace std;

namespace DW {
	namespace IMAGE {
				
		/**
			Represents a 2D pixel spacing
		 */
		class PixelSpacing2D {
		public:
			PixelSpacing2D()
			{
				x_ = 0.0;
				y_ = 0.0;
			}
			PixelSpacing2D(double x, double y)
			{
				x_ = x;
				y_ = y;
			}
			~PixelSpacing2D();
    
			/// Methods to set the values
			void SetX(double x)
			{
				x_ = x;
			}
			void SetY(double y)
			{
				y_ = y;
			}

			/// Methods to get the values
			double X() const
			{
				return x_;
			}
			double Y() const
			{
				return y_;
			}

			/// Tells if it contains valid pixel spacing values.
			/// Returns false if any of its components is negative or equals 0.0, true otherwise.
			bool IsValid() const
			{
				return x_ > 0.0 && y_ > 0.0;
			}

			/// Returns true if its values are equal to the given spacing
			/// By default only precision will be until 3 decimals
			/// If precision == -1 a standard double comparision will be performed
			bool IsEqual(const PixelSpacing2D &spacing, int precision = 3) const
			{
				if (precision < 0)
				{
					return x_ == spacing.X() && y_ == spacing.Y();
				}
				else
				{
					double left_x, left_y, right_x, right_y;
					left_x = x_;
					left_y = y_;
					right_x = spacing.X();
					right_y = spacing.Y();
					cout <<fixed<<setprecision(precision) << left_x <<endl;
					cout <<fixed<<setprecision(precision) << left_y <<endl;
					cout <<fixed<<setprecision(precision) << right_x <<endl;
					cout <<fixed<<setprecision(precision) << right_y <<endl;

					return (left_x - right_x) < MathTool::kEpsilon && (right_x - right_y) < MathTool::kEpsilon;
				}
			}

		private:
			/// Spacing values
			double x_;
			double y_;
		};

	}
}
