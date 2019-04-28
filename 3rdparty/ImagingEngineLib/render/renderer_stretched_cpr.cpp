/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_stretched_cpr.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "render/renderer_stretched_cpr.h"
#include "vtkImageEllipsoidSource.h"
#include "vtkLineSource.h"
#include "filter/spline_driven_image_slicer.h"
#include "tools/convert_vtk_image_data_to_rgba.h"
#include "render/render_param_cpr.h"
#include <vtkMath.h>
#include "tools/math.h"

using namespace DW::Render;

StretchedCPRRenderer::StretchedCPRRenderer()
{
	show_buffer_ = new ShowBuffer();
}

StretchedCPRRenderer::~StretchedCPRRenderer()
{
	if (show_buffer_) {
		delete show_buffer_;
		show_buffer_ = NULL;
	}
}

void StretchedCPRRenderer::DoRender()
{
	CPRRenderParam* param_imp = dynamic_cast<CPRRenderParam *>(render_param_);
	if (param_imp == NULL) return;
	VolCurve* curve = param_imp->GetCurve();
	if (curve == NULL) return;

	// 深拷贝参数？
	camera_ = param_imp->GetCamera();
	float direction[3];
	param_imp->GetDirection(direction[0], direction[1], direction[2]);
	float left_distance = 0.0f, right_distance = 0.0f;
	CalculateImageSize(param_imp->GetCurve(), direction, 1.0, left_distance, right_distance);
	// 缓存第一个采样点左右边距
	origin_padding_left_ = left_distance;
	origin_padding_right_ = right_distance;
	int width = (int)(left_distance + 0.5) + (int)(right_distance + 0.5);
	int height = curve->GetNumberOfSamplePoint();
	// 更新旋转中心点和旋转轴
	curve->GetCurveDirection(rotation_axis_[0], rotation_axis_[1], rotation_axis_[2]);
	param_imp->SetAxis(rotation_axis_[0], rotation_axis_[1], rotation_axis_[2]);
	float middle_sample[3] = {0};
	curve->GetSamplePoint(height / 2, middle_sample[0], middle_sample[1], middle_sample[2]);
	rotation_center_.x = middle_sample[0];
	rotation_center_.y = middle_sample[1];
	rotation_center_.z = middle_sample[2];
	param_imp->SetCenter(middle_sample[0], middle_sample[1], middle_sample[2]);
	// END

	int rows=height;
	int cols=width;

	show_buffer_->InitBufferData(cols, rows, 16);
	// 从vtk获取的ImageData像素值是经过偏移转换的，因此带着符号的：最大967，最小-3024
	// 因此此处用short类型取值
	short* ct_buffer = reinterpret_cast<short *>(show_buffer_->GetShowBuffer());

	double origins[3], spaces[3];
	vtkSmartPointer<vtkImageData> imagedata = volume_data_->GetVtkData();
	imagedata->GetOrigin(origins);
	imagedata->GetSpacing(spaces);
	vtkSmartPointer<vtkImageData> m_imageDataVTK = vtkSmartPointer<vtkImageData>::New();
	m_imageDataVTK->SetOrigin( origins );
	m_imageDataVTK->SetSpacing ( spaces );
	m_imageDataVTK->SetDimensions( cols, rows, 1 );
	m_imageDataVTK->SetScalarTypeToShort(); // the data will be 16 bit
	m_imageDataVTK->SetNumberOfScalarComponents(1);
	m_imageDataVTK->AllocateScalars();
	short* ptr = (short*)m_imageDataVTK->GetScalarPointer();

	float x[3];
	double distance_from_origin = 0.0;
	float origin[3], current_sample[3], projection[3];
	param_imp->GetCurve()->GetSamplePoint (0, origin[0], origin[1], origin[2]);
	int minval=100000, maxval = -100000;
	for (int row = 0; row < rows; row++)
	{
		param_imp->GetCurve()->GetSamplePoint (row, current_sample[0], current_sample[1], current_sample[2]);
		// 求解curpnt在向量LineOri上过prepnt点的投影点坐标、
		MathTool::Compute3DProjectionOnLine (direction, origin, current_sample, projection);
		// 计算投影距离,同向为正,反向为负
		float tmp_distance = MathTool::ComputeDistanceBetweenPoints (origin, projection, direction);
		for (int col = 0; col < cols; col++)
		{
			distance_from_origin = (col - origin_padding_left_) + tmp_distance;
			float p[3];
			param_imp->GetCurve()->GetSamplePoint (row, p[0], p[1], p[2]);
			x[0] = p[0] + direction[0] * distance_from_origin;
			x[1] = p[1] + direction[1] * distance_from_origin;
			x[2] = p[2] + direction[2] * distance_from_origin;
			short val = 0;
			TrilinearInterpolation(val, x[0], x[1], x[2], volume_data_);
			*(ptr + row * cols + col) = val;

			// 测试：找到最大最小CT值
			if (val < minval)
				minval = val;
			if (val > maxval)
				maxval = val;
		}
	}
		
	//memcpy((unsigned short *)m_imageDataVTK->GetScalarPointer(), reinterpret_cast<unsigned short *>(ct_buffer), rows*cols);
	//m_imageDataVTK->Modified();

	// 设置窗宽窗位的范围
	int ww = 0, wl = 0;
	// 从渲染参数对象中拿到窗宽窗位
	param_imp->GetWindowWidthLevel(ww, wl);
	vtkSmartPointer<vtkLookupTable> colorTable =
		vtkSmartPointer<vtkLookupTable>::New();
	// 窗宽窗位转换到最小最大值
	colorTable->SetRange(wl - ww / 2, wl + (ww + 1) / 2);
	colorTable->SetValueRange(0.0, 1.0);
	colorTable->SetSaturationRange(0.0, 0.0);		//灰度图像
	colorTable->SetRampToLinear();
	colorTable->Build();

	vtkSmartPointer<vtkImageMapToColors> colorMap =
		vtkSmartPointer<vtkImageMapToColors>::New();
	colorMap->SetLookupTable(colorTable);
	colorMap->SetInputConnection(m_imageDataVTK->GetProducerPort());
	colorMap->Update();

	//// convert to 32 bits bitmap
	//vtkImageData *pTmpImageData = NULL;	
	//ConvertVtkImagedataToRGBA* convert = new ConvertVtkImagedataToRGBA();
	//if(false == convert->ConvertImageScalarsToRGBA(m_imageDataVTK, &pTmpImageData, -1, 1500, -500))
	//{
	//	return;
	//}

	output_vtk_image_data_ = colorMap->GetOutput();
}
