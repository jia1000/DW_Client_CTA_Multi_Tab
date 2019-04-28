/*=========================================================================

  Program:   ImagingEngine
  Module:    math.h
  author: 	 zhangjian
  Brief:	 提供数值计算的方法

=========================================================================*/
#pragma once
#include <cmath>
#include <ctime>
#include <iostream>

class MathTool
{
public:
	///
	// 向量归一化
	///
	static float Normalize(float x[3])
	{
		float den;
		if ( ( den = Norm( x ) ) != 0.0 )
		{
			for (int i=0; i < 3; i++)
			{
				x[i] /= den;
			}
		}
		return den;
	}
	static float Normalize(Vector3f& x)
	{
		float den;
		if ( ( den = Norm( x ) ) != 0.0 )
		{
			for (int i=0; i < 3; i++)
			{
				x[i] /= den;
			}
		}
		return den;
	}
	///
	// 计算两点之间的像素距离
	///
	static float ComputeDistanceBetweenPoints(const float StartPnt[], const float EndPnt[])
	{	
		float fDistance;
		fDistance = sqrt((StartPnt[0] - EndPnt[0])*(StartPnt[0] - EndPnt[0]) + 
			(StartPnt[1]- EndPnt[1])*(StartPnt[1] - EndPnt[1]) + 
			(StartPnt[2] - EndPnt[2])*(StartPnt[2] - EndPnt[2]));
		return fDistance;
	}
	///
	// 计算两点之间的像素距离，带正负方向
	///
	static float ComputeDistanceBetweenPoints(const float StartPnt[], const float EndPnt[], const float direction[])
	{	
		double fDistance;
		fDistance = sqrt((StartPnt[0] - EndPnt[0])*(StartPnt[0] - EndPnt[0]) + 
			(StartPnt[1]- EndPnt[1])*(StartPnt[1] - EndPnt[1]) + 
			(StartPnt[2] - EndPnt[2])*(StartPnt[2] - EndPnt[2]));

		if (fDistance<1e-6)
			return 0.0f;

		float absx,absy,absz;
		absx = fabs(direction[0]);
		absy = fabs(direction[1]);
		absz = fabs(direction[2]);

		if ((absx >= absy) && (absx >= absz))
		{
			if ((EndPnt[0] - StartPnt[0])*direction[0]>0) 
				return  -fDistance;
			else
				return fDistance;
		}

		if ((absy >= absx) && (absy >= absz))
		{
			if ((EndPnt[1] - StartPnt[1])*direction[1]>0) 
				return -fDistance;
			else
				return fDistance;
		}

		if ((absz >= absx) && (absz >= absy))
		{
			if ((EndPnt[2] - StartPnt[2])*direction[2]>0) 
				return -fDistance;
			else
				return fDistance;
		}

		return 0.0f;
	}
	///
	// PL射线端点，PP平面一点，e射线单位向量，q平面信息，P返回交点
	///
	static void ComputeLineAndPlane(float PL[], float PP[], float e[], float* q, float* P)
	{
		float t;
		if((q[1]*e[0]+q[2]*e[1]+q[3]*e[2]) < 1e-5)
		{
			return;
		}
		else
		{
			t=(q[1]*(PP[0]-PL[0])+q[2]*(PP[1]-PL[1])+q[3]*PP[2])/(q[1]*e[0]+q[2]*e[1]+q[3]*e[2]);
			P[0]=PL[0]+e[0]*t;
			P[1]=PL[1]+e[1]*t;
			P[2]=e[2]*t;
		}
	}
	///
	// 求空间内一点在一条直线上的投影
	///
	static void Compute3DProjectionOnLine(const float line_vec[],
		const float pnt_on_line[],								  
		const float pnt_outside[],
		float *projection)
	{
		float tmp;
		tmp = line_vec[0] * (pnt_outside[0] - pnt_on_line[0]) + 
			line_vec[1] * (pnt_outside[1] - pnt_on_line[1]) + 
			line_vec[2] * (pnt_outside[2] - pnt_on_line[2]);
		tmp = tmp / (line_vec[0] * line_vec[0] + 
			line_vec[1] * line_vec[1] + 
			line_vec[2] * line_vec[2]);

		projection[0] = tmp * line_vec[0] + pnt_on_line[0];
		projection[1] = tmp * line_vec[1] + pnt_on_line[1];
		projection[2] = tmp * line_vec[2] + pnt_on_line[2];
	}
	///
	// 判断当前点在原点右侧还是左侧
	// 返回值：true右侧；false左侧
	///
	static bool IsPointOnTheRightSide(const float line_vec[],
		const float origin[],
		const float current_point[])
	{
		if ((current_point[0] - origin[0])*line_vec[0]>=0&&
			(current_point[1] - origin[1])*line_vec[1]>=0&&
			(current_point[2] - origin[2])*line_vec[2]>=0){
				return true;
		}else{
			return false;
		}
	}
	///
	// 绕轴旋转
	///
	static void rotate_axis(
		float u, float v, float w,        /*Axis*/
		float x, float y, float z,        /*The Point That We Want to Roate */
		float& nx, float& ny, float& nz,  /*Result*/
		float degree )
	{
		// change from degree to radian
		float A = degree * 3.14159265f / 180.0f;
		float c = cos(A);
		float s = sin(A);
		float C = 1.0f - c;

		if( std::abs(c) > 0.999 )
		{
			nx = x;
			ny = y;
			nz = z;
		}

		// Reference: http://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
		float Q[3][3];
		Q[0][0] = u * u * C + c;
		Q[0][1] = v * u * C + w * s;
		Q[0][2] = w * u * C - v * s;

		Q[1][0] = v * u * C - w * s;
		Q[1][1] = v * v * C + c;
		Q[1][2] = w * v * C + u * s;

		Q[2][0] = u * w * C + v * s;
		Q[2][1] = w * v * C - u * s;
		Q[2][2] = w * w * C + c;

		nx = x * Q[0][0] + y * Q[1][0] + z * Q[2][0];
		ny = x * Q[0][1] + y * Q[1][1] + z * Q[2][1];
		nz = x * Q[0][2] + y * Q[1][2] + z * Q[2][2];
	}

	//----------------------------------------------------------------------------
	// Cross product of two 3-vectors. Result (a x b) is stored in z[3].
	static void Cross(const float x[3], const float y[3], float z[3])
	{
		float Zx = x[1] * y[2] - x[2] * y[1];
		float Zy = x[2] * y[0] - x[0] * y[2];
		float Zz = x[0] * y[1] - x[1] * y[0];
		z[0] = Zx; z[1] = Zy; z[2] = Zz;
	}
	static void Cross(Vector3f x, Vector3f y, Vector3f& z)
	{
		float Zx = x[1] * y[2] - x[2] * y[1];
		float Zy = x[2] * y[0] - x[0] * y[2];
		float Zz = x[0] * y[1] - x[1] * y[0];
		z[0] = Zx; z[1] = Zy; z[2] = Zz;
	}

	// Description:
	// Compute the norm of 3-vector.
	static float Norm(const float x[3]) 
	{
		return static_cast<float> (sqrt( x[0] * x[0] + x[1] * x[1] + x[2] * x[2] ) );
	};
	static float Norm(Vector3f x) 
	{
		return static_cast<float> (sqrt( x[0] * x[0] + x[1] * x[1] + x[2] * x[2] ) );
	};
};