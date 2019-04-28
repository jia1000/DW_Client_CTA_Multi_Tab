/*=========================================================================

  Program:   ImagingEngine
  Module:    curve_line.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "data/curve_line.h"
#include "vtk_include.h"
#include "vtkPolyDataReader.h"
#include "vtkSplineFilter.h"
#include "vtkParametricSpline.h"
#include "vtkParametricFunctionSource.h"
#include "vtkCardinalSpline.h"
#include "tools/math.h"

using namespace DW::IMAGE;

VolCurve::VolCurve()
{
	using_consistent_normal_ = true;
}

VolCurve::~VolCurve()
{
}

// 添加一个三维控制点
void VolCurve::AddControlPoint(Point3f point)
{
	control_points_.push_back(point);
}
void VolCurve::AddControlPoint(float x, float y, float z)
{
	Point3f point;
	point.x = x;
	point.y = y;
	point.z = z;
	control_points_.push_back(point);
}
// 移除一个三维控制点
void VolCurve::RemoveControlPoint(int pos)
{
	if (control_points_.empty()) {
		return;
	}
	auto it = control_points_.begin();
	it += pos;
	if (it != control_points_.end())
	{
		control_points_.erase(it);
	}
}

void VolCurve::GetControlPoint(int pos, float& x, float& y, float& z)
{
	int point_count = control_points_.size();
	if(pos > point_count-1)
		return;
	x = control_points_[pos].x;
	y = control_points_[pos].y;
	z = control_points_[pos].z;
}

void VolCurve::GetSamplePoint(int pos, float& x, float& y, float& z)
{
	int point_count = sample_points_.size();
	if(pos > point_count-1)
		return;
	x = sample_points_[pos].x;
	y = sample_points_[pos].y;
	z = sample_points_[pos].z;
}

// 进行采样
void VolCurve::Update()
{
	//GenerateCurve();
	//return;

	sample_points_.clear();
	// 比如，进行cardinal样条插值
	string polyDataFileName="D:\\polydata.txt";
	int resolution = 374;
	// Read the Polyline
	// 控制点总数
	int control_points = GetNumberOfControlPoint();
	float tmp_point[3];
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
	for(int i=0;i<control_points-1;i++){
		tmp_point[0]=control_points_[i].x;
		tmp_point[1]=control_points_[i].y;
		tmp_point[2]=control_points_[i].z;
		points->InsertNextPoint(tmp_point);
	}
	vtkSmartPointer<vtkParametricSpline> splinepp =
		vtkSmartPointer<vtkParametricSpline>::New();
	splinepp->SetPoints(points);

	vtkSmartPointer<vtkParametricFunctionSource> functionSource =
		vtkSmartPointer<vtkParametricFunctionSource>::New();
	functionSource->SetParametricFunction(splinepp);
	functionSource->Update();

	vtkSmartPointer<vtkSplineFilter> spline =
		vtkSmartPointer<vtkSplineFilter>::New();
	spline->SetInputConnection(functionSource->GetOutputPort());
	spline->SetSubdivideToSpecified();
	spline->SetNumberOfSubdivisions(resolution);
	spline->Update();

	unsigned int rows = spline->GetOutput()->GetNumberOfPoints();
	for (unsigned int row = 0; row < rows; row++)
	{
		double p[3];
		spline->GetOutput()->GetPoint(row, p);
		Point3f pnt;
		pnt.x = p[0];
		pnt.y = p[1];
		pnt.z = p[2];
		sample_points_.push_back(pnt);
	}

	// 生成每个采样点切线向量以及局部平面坐标
	ComputeSampleVectors();
}

float VolCurve::CalculateDistance(int from_index, int to_index)
{
	int total_points = GetNumberOfSamplePoint();
	if(from_index <0)
		from_index = 0;
	if(to_index > total_points - 1)
		to_index = total_points - 1;
	float length =0;
	float x1,y1,z1,x2,y2,z2;
	for(int i=from_index; i<to_index; i++)
	{
		x1=sample_points_[i].x;
		y1=sample_points_[i].y;
		z1=sample_points_[i].z;

		x2=sample_points_[i+1].x;
		y2=sample_points_[i+1].y;
		z2=sample_points_[i+1].z;

		float len =sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1));
		length += len;
	}

	return length;
}

///
// 采用Cardinal插值生成曲线
///
void VolCurve::GenerateCurve()
{
	sample_points_.clear();
	// 控制点总数
	int control_points = GetNumberOfControlPoint();

	float tmp_point[3];
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
	for(int i=0;i<control_points-1;i++){
		tmp_point[0]=control_points_[i].x;
		tmp_point[1]=control_points_[i].y;
		tmp_point[2]=control_points_[i].z;
		
		points->InsertNextPoint(tmp_point);
	}

	vtkSmartPointer<vtkParametricSpline> spline =
		vtkSmartPointer<vtkParametricSpline>::New();
	spline->SetPoints(points);
	
	vtkSmartPointer<vtkParametricFunctionSource> functionSource =
		vtkSmartPointer<vtkParametricFunctionSource>::New();
	functionSource->SetParametricFunction(spline);
	functionSource->Update();

	vtkSmartPointer<vtkPolyData> line = functionSource->GetOutput();
	unsigned int rows = line->GetNumberOfPoints();
	double p[3];
	for (unsigned int row = 0; row < rows; row++)
	{
		line->GetPoint(row, p);
		Point3f sample_point;
		sample_point.x = p[0];
		sample_point.y = p[1];
		sample_point.z = p[2];
		sample_points_.push_back(sample_point);
	}
}

bool VolCurve::IsSame3DPoint(Point3f point)
{
	float x = point.x - sample_points_[sample_points_.size() - 1].x;
	float y = point.y - sample_points_[sample_points_.size() - 1].y;
	float z = point.z - sample_points_[sample_points_.size() - 1].z;
	if (fabs(x) < 0.0000001 && fabs(y) < 0.0000001 && fabs(z) < 0.0000001)
		return true;
	else
		return false;
}
///
// 获取曲线的趋势方向
// 目前以首尾点的方向来计算
///
void VolCurve::GetCurveDirection(float& x, float& y, float& z)
{
	int point_number = sample_points_.size();
	if (point_number < 1) return;

	Point3f first = sample_points_[0];
	Point3f last = sample_points_[point_number-1];

	float dir[3];
	dir[0] = last.x - first.x;
	dir[1] = last.y - first.y;
	dir[2] = last.z - first.z;
	MathTool::Normalize(dir);

	x = dir[0];
	y = dir[1];
	z = dir[2];
}

void VolCurve::ComputeSampleVectors()
{
	tangent_vectors_.clear();
	normal_vectors_.clear();
	binormal_vectors_.clear();

	int sample_count = GetNumberOfSamplePoint();
	// compute tangent vectors
	for(int i = 0 ; i<sample_count ; ++i){
		Vector3f tangent;
		if( i == 0 )
			this->ComputeTangentVector( sample_points_[0], sample_points_[1], tangent );
		else if( i == sample_count-1 )
			this->ComputeTangentVector( sample_points_[sample_count-2], sample_points_[sample_count-1],tangent );
		else
			this->ComputeTangentVector( sample_points_[i], sample_points_[i+1],tangent );

		MathTool::Normalize(tangent);
		// add to vector list
		tangent_vectors_.push_back(tangent);
	}
	// compute normal and binormal vectors
	for( int i = 0 ; i<sample_count; ++i)
	{
		Vector3f normal;
		if( !this->using_consistent_normal_ || i == 0){
			Vector3f tangentLast, tangentNext;
			if ( i == 0 ){
				tangentLast = tangent_vectors_[0];
			}
			else{
				tangentLast = tangent_vectors_[i-1];
			}
			if ( i == sample_count-1 ){
				tangentNext = tangent_vectors_[i];
			}
			else{
				tangentNext = tangent_vectors_[i];	//i+1
			}
			ComputeNormalVector( tangentLast, tangentNext, normal );
		}

		if( this->using_consistent_normal_ && i != 0)
		{

			Vector3f tangent, lastNormal;
			lastNormal = normal_vectors_[i-1];
			tangent = tangent_vectors_[i];

			ComputeConsistentNormalVector( tangent, lastNormal, normal );
		}

		MathTool::Normalize( normal );
		// add to vector list
		normal_vectors_.push_back(normal);
		// compute binormal vector
		Vector3f tangent, binormal;
		tangent = tangent_vectors_[i];
		MathTool::Cross( tangent, normal, binormal );
		MathTool::Normalize( binormal );
		// add to vector list
		binormal_vectors_.push_back(binormal);
	}
}

void VolCurve::ComputeTangentVector(Point3f last_pnt, Point3f next_pnt, Vector3f& tangent)
{
	tangent[0] = next_pnt.x - last_pnt.x;
	tangent[1] = next_pnt.y - last_pnt.y;
	tangent[2] = next_pnt.z - last_pnt.z;
}

void VolCurve::ComputeNormalVector(Vector3f last_tangent, Vector3f next_tangent, Vector3f& normal)
{
	int comp;
	for( comp = 0; comp < 3; comp++ )
		normal[comp] = ( next_tangent[comp] - last_tangent[comp] );

	if( MathTool::Norm(normal) == 0 )	// last_tangent == next_tangent
	{
		Vector3f unit(1,0,0);
		MathTool::Cross( last_tangent, unit, normal );
	}
}

void VolCurve::ComputeConsistentNormalVector(Vector3f tangent, Vector3f last_normal, Vector3f& normal)
{
	Vector3f temp;
	MathTool::Cross( last_normal, tangent, temp);
	MathTool::Cross( tangent, temp, normal );
}

void VolCurve::GetTangentVector(int index, Vector3f& tangent)
{
	if (index < tangent_vectors_.size()){
		tangent = tangent_vectors_[index];
	}
}

void VolCurve::GetLocalPlaneVector(int index, Vector3f& x, Vector3f& y)
{
	if (index < normal_vectors_.size()){
		x = normal_vectors_[index];
	}
	if (index < binormal_vectors_.size()){
		y = binormal_vectors_[index];
	}
}