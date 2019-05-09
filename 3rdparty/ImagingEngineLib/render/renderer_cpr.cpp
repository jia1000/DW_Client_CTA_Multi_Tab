/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_cpr.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "render/renderer_cpr.h"
#include "renderer_raycasting_cpu.h"
#include <vtkWindowToImageFilter.h>
#include <vtkBMPWriter.h>
#include <vtkNew.h>
#include <vtkCamera.h>
#include <vtkColorTransferFunction.h>
#include <vtkDataArray.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkImageData.h>
#include <vtkImageReader.h>
#include <vtkImageShiftScale.h>
#include <vtkNew.h>
#include <vtkOutlineFilter.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkStructuredPointsReader.h>
#include <vtkTimerLog.h>
#include <vtkVolumeProperty.h>
#include <vtkDICOMImageReader.h>
#include <vtkCallbackCommand.h>

//#include "tools/bitmap_writer.h"
#include "tools/math.h"
#include "render/render_param_cpr.h"
#include "tools/convert_vtk_image_data_to_rgba.h"
#include "tools/timer.h"
#include "tools/logger.h"
#include "processing/image_process.h"

using namespace DW::IMAGE;
using namespace DW::Render;
using namespace DW::CV;

int CPRRenderer::tmp_counter_ = 0;

CPRRenderer::CPRRenderer()
{
	render_window_ = vtkSmartPointer<vtkRenderWindow>::New();
	is_first_render_ = true;
	show_buffer_ = new ShowBuffer();
	tmp_counter_++;
	cpr_file_id = tmp_counter_;
}
CPRRenderer::~CPRRenderer()
{

}
void CPRRenderer::Render()
{
	if (volume_data_ == NULL) return;

	DoRender();
}
ShowBuffer *CPRRenderer::GetShowBuffer()
{
	return show_buffer_;
}
void CPRRenderer::SetData(VolData* data)
{
	volume_data_ = data;
}

void CPRRenderer::DoRender()
{

}

vtkSmartPointer<vtkPolyData> CPRRenderer::SweepLine (VolCurve *curve,
										float direction[3],
										float distance,
										unsigned int cols)
{
	unsigned int rows = curve->GetNumberOfControlPoint();
	float spacing = distance / cols;
	vtkSmartPointer<vtkPolyData> surface =
		vtkSmartPointer<vtkPolyData>::New();

	// Generate the points
	cols++;
	unsigned int numberOfPoints = rows * cols;
	unsigned int numberOfPolys = (rows - 1) * (cols - 1);
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
	points->Allocate(numberOfPoints);
	vtkSmartPointer<vtkCellArray> polys =
		vtkSmartPointer<vtkCellArray>::New();
	polys->Allocate(numberOfPolys*4);

	float x[3];
	unsigned int cnt = 0;
	for (unsigned int row = 0; row < rows; row++)
	{
		for (unsigned int col = 0; col < cols; col++)
		{
			float p[3];
			curve->GetControlPoint(row, p[0], p[1], p[2]);
			x[0] = p[0] + direction[0] * col * spacing;
			x[1] = p[1] + direction[1] * col * spacing;
			x[2] = p[2] + direction[2] * col * spacing;
			points->InsertPoint(cnt++, x);
		}
	}
	// Generate the quads
	vtkIdType pts[4];
	for (unsigned int row = 0; row < rows - 1; row++)
	{
		for (unsigned int col = 0; col < cols - 1; col++)
		{
			pts[0] = col + row * (cols);
			pts[1] = pts[0] + 1;
			pts[2] = pts[0] + cols + 1;
			pts[3] = pts[0] + cols;
			polys->InsertNextCell(4,pts);
		}
	}
	surface->SetPoints(points);
	surface->SetPolys(polys);

	return surface;
}


vtkSmartPointer<vtkPolyData> CPRRenderer::SweepLine2 (vtkPolyData *line,
													 float direction[3],
													 float distance,
													 unsigned int cols)
{
	unsigned int rows = line->GetNumberOfPoints();
	float spacing = distance / cols;
	vtkSmartPointer<vtkPolyData> surface =
		vtkSmartPointer<vtkPolyData>::New();

	// Generate the points
	cols++;
	unsigned int numberOfPoints = rows * cols;
	unsigned int numberOfPolys = (rows - 1) * (cols - 1);
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
	points->Allocate(numberOfPoints);
	vtkSmartPointer<vtkCellArray> polys =
		vtkSmartPointer<vtkCellArray>::New();
	polys->Allocate(numberOfPolys*4);

	double x[3];
	unsigned int cnt = 0;
	for (unsigned int row = 0; row < rows; row++)
	{
		for (unsigned int col = 0; col < cols; col++)
		{
			double p[3];
			line->GetPoint(row, p);
			x[0] = p[0] + direction[0] * col * spacing;
			x[1] = p[1] + direction[1] * col * spacing;
			x[2] = p[2] + direction[2] * col * spacing;
			points->InsertPoint(cnt++, x);
		}
	}
	// Generate the quads
	vtkIdType pts[4];
	for (unsigned int row = 0; row < rows - 1; row++)
	{
		for (unsigned int col = 0; col < cols - 1; col++)
		{
			pts[0] = col + row * (cols);
			pts[1] = pts[0] + 1;
			pts[2] = pts[0] + cols + 1;
			pts[3] = pts[0] + cols;
			polys->InsertNextCell(4,pts);
		}
	}
	surface->SetPoints(points);
	surface->SetPolys(polys);

	return surface;
}


void CPRRenderer::SetOffScreenRendering(bool flag)
{
	is_off_screen_rendering_ = flag;
}

bool CPRRenderer::PickByImagePos(const Vector2i& in_point, Vector3d& out_point)
{
	return false;
}

bool CPRRenderer::PickByImagePos(const int& x, const int& y, Vector3d& out_point)
{
	return false;
}

bool CPRRenderer::PickBy3DPoint(const Vector3d& in_point, int& x, int& y)
{
	return false;
}

float CPRRenderer::CalculateLength(Vector3d&, Vector3d&)
{
	return 0.0;
}

void CPRRenderer::GetOrientationTags(Vector3d&, Vector3d&, Vector3d&, Vector3d&)
{

}
///
// 计算直线与包围盒的两个交点
///
int CPRRenderer::Compute3DCrossPoint(const float curr_point[],
						 float direction[],
						 BoundingBox * box,
						 float *cross_point_one, 
						 float *cross_point_two)
{
	int width = box->GetXLength();
	int height = box->GetYLength();
	int slice_number = box->GetZLength();
	float tmp_point[3];
	int counter = 0;
	int length_arr[3] = { width, height, slice_number };

	for (int i=0; i<3; ++i) {
		tmp_point[0] = i==0 ? 0 : (-curr_point[i]) * direction[0] / direction[i] + curr_point[0];
		tmp_point[1] = i==1 ? 0 : (-curr_point[i]) * direction[1] / direction[i] + curr_point[1];
		tmp_point[2] = i==2 ? 0 : (-curr_point[i]) * direction[2] / direction[i] + curr_point[2];

		if (box->IsPointInBox(tmp_point[0], tmp_point[1], tmp_point[2])) {
			if (++counter == 1) {
				cross_point_one[0] = tmp_point[0];
				cross_point_one[1] = tmp_point[1];
				cross_point_one[2] = tmp_point[2];
			}
			else {
				cross_point_two[0] = tmp_point[0];
				cross_point_two[1] = tmp_point[1];
				cross_point_two[2] = tmp_point[2];
			}
		}

		tmp_point[0] = i==0 ? length_arr[i] - 1 : (length_arr[i] - 1 -curr_point[i])*direction[0] / direction[i] + curr_point[0];
		tmp_point[1] = i==1 ? length_arr[i] - 1 : (length_arr[i] - 1 -curr_point[i])*direction[1] / direction[i] + curr_point[1];
		tmp_point[2] = i==2 ? length_arr[i] - 1 : (length_arr[i] - 1 -curr_point[i])*direction[2] / direction[i] + curr_point[2];

		if (box->IsPointInBox(tmp_point[0], tmp_point[1], tmp_point[2])) {
			if (++counter == 1) {
				cross_point_one[0] = tmp_point[0];
				cross_point_one[1] = tmp_point[1];
				cross_point_one[2] = tmp_point[2];
			}
			else {
				cross_point_two[0] = tmp_point[0];
				cross_point_two[1] = tmp_point[1];
				cross_point_two[2] = tmp_point[2];
			}
		}
	}
	
	return counter;
}

///
// 三线性插值
///
void CPRRenderer::TrilinearInterpolation(short& ctv, float ix, float iy, float iz, VolData *data)
{
	short gO,gP,gQ,gR,gS,gT,gU,gV,gA,gB,gC,gD,gE,gF; 
	int nnx,nny,nnz;
	nnx = (int)ix;
	nny = (int)iy;
	nnz = MathTool::Round(iz);
	int width = data->GetSliceWidth();
	int slice_number = data->GetSliceCount();
	int height = data->GetSliceHeight();
	if (nnx < 0 || nnx > width-2 || nny < 0 || nny >height-2 || nnz < 0 || nnz > slice_number - 1) {
		ctv = 0;	
		return;
	}
	int nID = nny*width+nnx;
	int nID1 = (nny+1)*width+nnx;
	double dZ = (iz-nnz);
	// 最后一层不超出范围
	int nnz_next = nnz < slice_number-1 ? nnz+1 : nnz-1;
	gO = *(reinterpret_cast<short *>(data->GetDataPointer(nnx, nny, nnz)));
	gP = *(reinterpret_cast<short *>(data->GetDataPointer(nnx, nny, nnz_next)));
	gQ = *(reinterpret_cast<short *>(data->GetDataPointer(nnx+1, nny, nnz)));
	gR = *(reinterpret_cast<short *>(data->GetDataPointer(nnx+1, nny, nnz_next)));
	gS = *(reinterpret_cast<short *>(data->GetDataPointer(nnx, nny+1, nnz)));
	gT = *(reinterpret_cast<short *>(data->GetDataPointer(nnx, nny+1, nnz_next)));
	gU = *(reinterpret_cast<short *>(data->GetDataPointer(nnx+1, nny+1, nnz)));
	gV = *(reinterpret_cast<short *>(data->GetDataPointer(nnx+1, nny+1, nnz_next)));
	
	gA = (short)(dZ*(gP-gO))+gO;
	gB = (short)(dZ*(gR-gQ))+gQ;
	gC = (short)(dZ*(gT-gS))+gS;
	gD = (short)(dZ*(gV-gU))+gU;
	gE = (short)((ix-nnx)*(gB-gA))+gA;
	gF = (short)((ix-nnx)*(gD-gC))+gC;
	ctv = (short)((iy-nny)*(gF-gE))+gE;
}

void CPRRenderer::UpdateRotation()
{
	
}

void CPRRenderer::SetRotationAxis(float x, float y, float z)
{
	rotation_axis_[0] = x;
	rotation_axis_[1] = y;
	rotation_axis_[2] = z;
}

void CPRRenderer::GetRotationAxis(float& x, float& y, float& z)
{
	x = rotation_axis_[0];
	y = rotation_axis_[1];
	z = rotation_axis_[2];
}

void CPRRenderer::GetRotationCenter(float& x, float& y, float& z)
{
	x = rotation_center_.x;
	y = rotation_center_.y;
	z = rotation_center_.z;
}

void CPRRenderer::CalculateImageSize(VolCurve *curve,
											  float direction[3],
											  float thickness,
											  float& left_distance,
											  float& right_distance)
{
	BoundingBox* box = volume_data_->GetBoundingBox();
	MathTool::Normalize(direction);

	int total_count = curve->GetNumberOfSamplePoint();
	// 得到第一个点的坐标
	float origin[3] = {0,0,0};
	curve->GetSamplePoint(0, origin[0], origin[1], origin[2]);	

	float curr_point[3], curr_proj_point[3], cross_point1[3],cross_point2[3];
	cross_point1[0] = cross_point1[1] = cross_point1[2] =0;
	cross_point2[0] = cross_point2[1] = cross_point2[2] =0;

	for(int i=1; i<total_count; i++){		
		curve->GetSamplePoint(i, curr_point[0], curr_point[1], curr_point[2]);
		if (!box->IsPointInBox(curr_point[0], curr_point[1], curr_point[2])){
			// 包围盒不会更改，也不会出现这种情况...
		}

		int cross_count = Compute3DCrossPoint(curr_point, direction, box, cross_point1, cross_point2);
		if (cross_count >= 2){
			GetBoundingBoxBorder(cross_point1, direction, origin, curr_proj_point, left_distance, right_distance);
			GetBoundingBoxBorder(cross_point2, direction, origin, curr_proj_point, left_distance, right_distance);
		}
	}
}

void CPRRenderer::GetBoundingBoxBorder(float cross_pnt[3],
												float direction[3],
												float origin[3],
												float curr_proj_point[3],
												float& left_distance, 
												float& right_distance)
{
	// 计算包围盒交点在第一个采样点直线方向上的投影点
	MathTool::Compute3DProjectionOnLine(direction,origin,cross_pnt,curr_proj_point);
	// 计算投影点与第一个采样点之间的距离
	float tmp_distance = MathTool::ComputeDistanceBetweenPoints(origin,curr_proj_point);

	if (MathTool::IsPointOnTheRightSide(direction, origin, curr_proj_point)){
		if (tmp_distance > right_distance)
			right_distance = tmp_distance;
	}
	else{
		if (tmp_distance > left_distance)
			left_distance = tmp_distance;
	}
}

void CPRRenderer::ComputeWorldToDisplay(Point3f& world_pos, Point3f& display_pos)
{

}

void CPRRenderer::ComputeWorldToDisplay(double x, double y, double z, double display_point[3])
{

}

void CPRRenderer::BufferTransform()
{
	if (output_vtk_image_data_ && show_buffer_){
		
		/// Apply window width/level
		int ww = 0, wl = 0;
		/// 从渲染参数对象中拿到窗宽窗位
		CPRRenderParam* param_imp = dynamic_cast<CPRRenderParam *>(render_param_);
		if (param_imp == NULL){
			return;
		}
		param_imp->GetWindowWidthLevel(ww, wl);
		vtkSmartPointer<vtkLookupTable> colorTable =
			vtkSmartPointer<vtkLookupTable>::New();
		/// 窗宽窗位转换到最小最大值
		colorTable->SetRange(wl - ww / 2, wl + (ww + 1) / 2);
		colorTable->SetValueRange(0.0, 1.0);
		colorTable->SetSaturationRange(0.0, 0.0);		//灰度图像
		colorTable->SetRampToLinear();
		colorTable->Build();

		vtkSmartPointer<vtkImageMapToColors> colorMap =
			vtkSmartPointer<vtkImageMapToColors>::New();
		colorMap->SetLookupTable(colorTable);
#if VTK_MAJOR_VERSION > 5
		colorMap->SetInputData(output_vtk_image_data_);
#else
		colorMap->SetInput(output_vtk_image_data_);
#endif	
		colorMap->Update();
		vtkSmartPointer<vtkImageData> formatted_image_data = colorMap->GetOutput();
		if (formatted_image_data == NULL){
			return;
		}

		//原来的方法，用来对比生成的图像
		int width = formatted_image_data->GetDimensions()[0];
		int height = formatted_image_data->GetDimensions()[1];		
		// 从8位转换到32位
		int slice = -1;

		UNITDATASHOW* pdata = reinterpret_cast<UNITDATASHOW*>( formatted_image_data->GetScalarPointer() );
		int number_of_components = formatted_image_data->GetNumberOfScalarComponents();

		show_buffer_->SetBufferData(pdata, width, height, number_of_components * 8);

	}
}