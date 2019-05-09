/*=========================================================================

  Program:   ImagingEngine
  Module:    math.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "tools/math.h"

/// 定义常量
// log_2 e
const double MathTool::kNumberEBase2Logarithm = 1.4426950408889634074;
// 1/pi
const double MathTool::kReversePiAsDouble = 0.31830988618379067;
// pi
const float MathTool::kPi = 3.14159265f;
// pi
const long double MathTool::kPiAsDouble = 3.14159265358979323846;
const double MathTool::kEpsilon = 1E-9;
const double MathTool::kDegreesToRadiansAsDouble = 0.017453292519943295;
const double MathTool::kRadiansToDegreesAsDouble = 57.29577951308232;

///
// 向量归一化
///
float MathTool::Normalize(float x[3])
{
	float den;
	if ( ( den = MathTool::Norm( x ) ) != 0.0 )
	{
		for (int i=0; i < 3; i++)
		{
			x[i] /= den;
		}
	}
	return den;
}
///
// 向量归一化
///
float MathTool::Normalize(Vector3f& x)
{
	float den;
	if ( ( den = MathTool::Norm( x ) ) != 0.0 )
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
float MathTool::ComputeDistanceBetweenPoints(const float firstPoint[], const float secondPoint[])
{	
	float xx = firstPoint[0] - secondPoint[0];
	float yy = firstPoint[1] - secondPoint[1];
	float zz = firstPoint[2] - secondPoint[2];
	float value = pow(xx, 2) + pow(yy, 2) + pow(zz, 2);
	return sqrt(value);
}
///
// 计算两点之间的像素距离，带正负方向
///
float MathTool::ComputeDistanceBetweenPoints(const float StartPnt[3], const float EndPnt[3], const float direction[3])
{	
	float fDistance;
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
void MathTool::ComputeLineAndPlane(float PL[], float PP[], float e[], float* q, float* P)
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
void MathTool::Compute3DProjectionOnLine(const float line_vec[],
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
bool MathTool::IsPointOnTheRightSide(const float line_vec[],
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
void MathTool::RotateAroundAxis(
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
void MathTool::Cross(const float x[3], const float y[3], float z[3])
{
	float Zx = x[1] * y[2] - x[2] * y[1];
	float Zy = x[2] * y[0] - x[0] * y[2];
	float Zz = x[0] * y[1] - x[1] * y[0];
	z[0] = Zx; z[1] = Zy; z[2] = Zz;
}
void MathTool::Cross(Vector3f x, Vector3f y, Vector3f& z)
{
	float Zx = x[1] * y[2] - x[2] * y[1];
	float Zy = x[2] * y[0] - x[0] * y[2];
	float Zz = x[0] * y[1] - x[1] * y[0];
	z[0] = Zx; z[1] = Zy; z[2] = Zz;
}
void MathTool::Cross(Vector3d x, Vector3d y, Vector3d& z)
{
	double Zx = x[1] * y[2] - x[2] * y[1];
	double Zy = x[2] * y[0] - x[0] * y[2];
	double Zz = x[0] * y[1] - x[1] * y[0];
	z[0] = Zx; z[1] = Zy; z[2] = Zz;
}

float MathTool::DotProduct(const float vector1[3], const float vector2[3])
{
	return ((vector1[0] * vector2[0]) + (vector1[1] * vector2[1]) + (vector1[2] * vector2[2]));
}

// Description:
// Compute the norm of 3-vector.
float MathTool::Norm(const float x[3]) 
{
	return static_cast<float> (sqrt( x[0] * x[0] + x[1] * x[1] + x[2] * x[2] ) );
};
float MathTool::Norm(Vector3f x) 
{
	return static_cast<float> (sqrt( x[0] * x[0] + x[1] * x[1] + x[2] * x[2] ) );
};

float MathTool::DegreesToRadians(float degrees)
{
	return (degrees * kPi) / 180.0f;
}

float MathTool::RadiansToDegrees(float radians)
{
	return (radians * 180.0f) / kPi;
}

float MathTool::AngleInRadians(const Vector3f &vec1, const Vector3f &vec2)
{
	return acos(Dot(vec1, vec2) / (vec1.length() * vec2.length()));
}

float MathTool::AngleInDegrees(const Vector3f &vec1, const Vector3f &vec2)
{
	return (float)(AngleInRadians(vec1, vec2) * MathTool::kRadiansToDegreesAsDouble);
}

int MathTool::PlaneIntersection(float p[3], float n[3], float q[3], float m[3], float r[3], float t[3])
{
	if (MathTool::AngleInDegrees(Vector3f(n[0], n[1], n[2]), Vector3f(m[0], m[1], m[2])) == 0.0)
	{
		return 0;
	}
	// Solució extreta de http://vis.eng.uci.edu/courses/eecs104/current/GraphicsMath.pdf, pàg. 64
	// pla1 definit per (p,n); p: punt del pla, p.ex. origen; n: normal
	// pla2 definit per (q,m); q: punt del pla, p.ex. origen; m: normal
	// línia d'intersecció (r,t); r: punt de la recta que pertany a tots dos plans; t: vector director
	// u: vector perpendicular a n i t;
	// Cross: producte vectorial
	// Dot: producte escalar
	// * : multiplicació de vectors
	// + : suma de vectors
	//
	// ******* FORMULETA *************
	//
	// t = Cross(n,m)
	// u = Cross(n,t)
	// r = p + Dot(p-q)*u / Dot(u,m)

	float u[3];
	MathTool::Cross(n, m, t);
	MathTool::Cross(n, t, u);

	float pq[3], sum[3], pqDotm, dot_u_m;

	pq[0] = q[0] - p[0];
	pq[1] = q[1] - p[1];
	pq[2] = q[2] - p[2];

	pqDotm = MathTool::DotProduct(pq, m);
	dot_u_m = MathTool::DotProduct(u, m);
	sum[0] = (pqDotm * u[0]) / dot_u_m;
	sum[1] = (pqDotm * u[1]) / dot_u_m;
	sum[2] = (pqDotm * u[2]) / dot_u_m;

	r[0] = sum[0] + p[0];
	r[1] = sum[1] + p[1];
	r[2] = sum[2] + p[2];

	return 1;
}

bool MathTool::ComputeRotationAngleAxis(const Vector3f &old_vector, 
										 const Vector3f &new_vector, 
										 float &angle, Vector3f &axis)
{
	// 计算点积
	double dot_val = ((old_vector[0] * new_vector[0]) + 
		(old_vector[1] * new_vector[1]) + 
		(old_vector[2] * new_vector[2]));
	// P * Q = |p| * |Q| * cos(angle)
	// 由以上点积公式可以求出两个向量的夹角angle
	//double old_norm = sqrt(old_vector.x * old_vector.x + 
	//	old_vector.y * old_vector.y + 
	//	old_vector.z * old_vector.z);
	//double new_norm = sqrt(new_vector.x * new_vector.x + 
	//	new_vector.y * new_vector.y + 
	//	new_vector.z * new_vector.z);
	angle = (float)acos(dot_val / old_vector.length() * new_vector.length());

	// 计算旋转轴
	axis[0] = old_vector[1] * new_vector[2] - old_vector[2] * new_vector[1];
	axis[1] = old_vector[2] * new_vector[0] - old_vector[0] * new_vector[2];
	axis[2] = old_vector[0] * new_vector[1] - old_vector[1] * new_vector[0];

	return true;
}