/*=========================================================================

  Program:   ImagingEngine
  Module:    color4.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data_definition.h"

using namespace std;

#define _GMAX(a,b) ( a > b? a : b )
#define _GMIN(a,b) ( a < b? a : b )

namespace DW {
	namespace IMAGE{

		class Color4 
		{
		private:
			float R;
			float G;
			float B;
			float A;

		public:

			inline Color4() {
				R = 1.0f;
				G = 1.0f;
				B = 1.0f;
				A = 1.0f;
			}

			inline Color4(const Color4& c)
			{
				*this = c;
			}

			inline Color4 (float r, float g, float b, float a = 1.0f)
			{
				R = Clamp(r);
				G = Clamp(g);
				B = Clamp(b);
				A = Clamp(a);
			}

			inline Color4 (unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
			{
				R = Clamp((float)r / 255.0f);
				G = Clamp((float)g / 255.0f);
				B = Clamp((float)b / 255.0f);
				A = Clamp((float)a / 255.0f);
			}

			inline Color4& operator=(const Color4& o)
			{
				R = o.R;
				G = o.G;
				B = o.B;
				A = o.A;
				return *this;
			}

			inline bool operator==(const Color4& o)
			{
				return (R == o.R && G == o.G && B == o.B && A == o.A);
			}
			
			inline bool operator!=(const Color4& o)
			{
				return !( (*this) == o);
			}

			inline Color4 operator*(float val) const
			{
				Color4 c(*this);
				c.R = Clamp(c.R * val);
				c.G = Clamp(c.G * val);
				c.B = Clamp(c.B * val);
				c.A = Clamp(c.A * val);
				return c;
			}

			inline Color4 operator-(float val) const
			{
				Color4 c(*this);
				c.R = Clamp(c.R - val);
				c.G = Clamp(c.G - val);
				c.B = Clamp(c.B - val);
				return c;
			}

			inline Color4 operator-(const Color4& o) const
			{
				Color4 c(*this);
				c.R = Clamp(c.R - o.R);
				c.G = Clamp(c.G - o.G);
				c.B = Clamp(c.B - o.B);
				return c;
			}

			inline Color4 operator/(float val) const
			{
				Color4 c(*this);
				c.R = Clamp(c.R / val);
				c.G = Clamp(c.G / val);
				c.B = Clamp(c.B / val);
				c.A = Clamp(c.A / val);
				return c;
			}

			inline Color4 Complementario() const
			{
				float metrica = Max() + Min();
				return (*this) - metrica;
			}

			inline Color4 Complementar()
			{
				return *this = Complementario();
			}

			inline Color4 Inverso() const
			{
				return Color4(1.0f, 1.0f, 1.0f) - (*this);
			}

			inline float Clamp(float valor) const
			{
				return _GMAX(0.0f, _GMIN(1.0f, valor));
			}

			inline float Max() const
			{
				return _GMAX(R, _GMAX(G, B));
			}

			inline float Min() const
			{
				return _GMIN(R, _GMIN(G, B));
			}

			inline Color4& Invertir() {
				return (*this = Inverso());
			}

			inline unsigned char R8() const
			{
				return (unsigned char) (this->R * 255.0f);
			}

			inline unsigned char G8() const
			{
				return (unsigned char) (G * 255.0f);
			}

			inline unsigned char B8() const
			{
				return (unsigned char) (B * 255.0f);
			}

			inline unsigned char A8() const
			{
				return (unsigned char) (A * 255.0f);
			}

			inline float RF() const
			{
				return R;
			}

			inline float GF() const
			{
				return G;
			}

			inline float BF() const
			{
				return B;
			}

			inline float AF() const
			{
				return A;
			}

			inline void SetRF(float r)
			{
				R = Clamp(r);
			}

			inline void SetGF(float g)
			{
				G = Clamp(g);
			}

			inline void SetBF(float b)
			{
				B = Clamp(b);
			}

			inline void SetAF(float a)
			{
				A = Clamp(a);
			}

			inline void SetR8(unsigned char r)
			{
				R = Clamp((float)r / 255.0f);
			}

			inline void SetG8(unsigned char g)
			{
				G = Clamp((float)g / 255.0f);
			}

			inline void SetB8(unsigned char b)
			{
				B = Clamp((float)b / 255.0f);
			}

			inline void SetA8(unsigned char a)
			{
				A = Clamp((float)a / 255.0f);
			}

			//friend std::ostream& operator<<(std::ostream& out, const Color4& c)
			//{
			//	return out << "[ R = " << c.R << ", G = " << c.G << ", B = " << c.B << ", A = " << c.A << " ]";
			//}

			//friend std::ostream& operator<<(std::ostream& out, const Color4* c)
			//{
			//	if (c != NULL) {
			//		return out << *c;
			//	}
			//	else {
			//		return out << "[ NULL ]";
			//	}
			//}
		};

	}
}
