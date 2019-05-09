/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_mpr.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "render/renderer_mpr.h"
#include "render/render_param_mpr.h"
#include "tools/vtk_image_data_creator.h"
#include "tools/convert_vtk_image_data_to_rgba.h"
#include "tools/math.h"
#include "tools/timer.h"
#include "tools/logger.h"
#include "processing/image_process.h"

using namespace std;
using namespace DW::IMAGE;
using namespace DW::Render;
using namespace DW::CV;

int MPRRenderer::tmp_counter_ = 0;

MPRRenderer::MPRRenderer()
{
	render_window_ = vtkSmartPointer<vtkRenderWindow>::New();
	is_first_render_ = true;
	show_buffer_ = new ShowBuffer();

	tmp_counter_++;
	mpr_file_id = tmp_counter_;
	dump_file_name_ = "D:\\mpr_" + to_string(mpr_file_id) + ".bmp";
}
MPRRenderer::~MPRRenderer()
{

}
void MPRRenderer::Render()
{
	if (volume_data_ == NULL) return;

	DoRender();
}
ShowBuffer *MPRRenderer::GetShowBuffer()
{
	/// Convert vtkImageData to Showbuffer object
	BufferTransform();

	return show_buffer_;
}
void MPRRenderer::SetData(VolData* data)
{
	volume_data_ = data;
}

void MPRRenderer::DoRender()
{
	//Timer::begin("MPR::DoRender");

	MPRRenderParam* param_imp = dynamic_cast<MPRRenderParam *>(render_param_);
	if (param_imp == NULL) return;

	// 图像属性
	int image_width = volume_data_->GetSliceWidth();
	int image_height = volume_data_->GetSliceHeight();
	int image_count = volume_data_->GetSliceCount();
	BoundingBox *box = volume_data_->GetBoundingBox();

	// MPR图像宽高
	int width = param_imp->GetWidth();
	int height = param_imp->GetHeight();
	// 层厚设置
	float thickness = param_imp->GetThickness();
	float half_thickness = thickness * 0.5f;
	// 行列向量
	Vector3f row_vector, col_vector, normal_vector;
	param_imp->GetPlaneVector(row_vector, col_vector);
	// 平面法向量
	MathTool::Cross(row_vector, col_vector, normal_vector);
	// 图像中心点
	Point3f center_point;
	param_imp->GetImageCenter(center_point);

	// 初始化buffer
	show_buffer_->InitBufferData(width, height, 16);
	// 从vtk获取的ImageData像素值是经过偏移转换的，因此带着符号的：最大967，最小-3024
	// 因此此处用short类型取值
	short* ct_buffer = reinterpret_cast<short *>(show_buffer_->GetShowBuffer());
	double origins[3], spaces[3];
	int dim[3];
	volume_data_->GetPixelData()->GetVtkImageData()->GetOrigin(origins);
	volume_data_->GetPixelData()->GetVtkImageData()->GetSpacing(spaces);
	dim[0] = width;
	dim[1] = height;
	dim[2] = 1;
	short* ptr = NULL;
	VtkImageDataCreator imageDataCreator;
	imageDataCreator.SetOrigin(origins);
	imageDataCreator.SetSpacing(spaces);
	imageDataCreator.SetDimensions(dim);
	imageDataCreator.SetNumberOfComponents(1);
	output_vtk_image_data_ = imageDataCreator.Create(ptr);
	ptr = (short*)output_vtk_image_data_->GetScalarPointer();

	//Timer::begin("MPR::DoRender::Loop");

	float x, y, z;
	float x1,y1,z1;
	for (int i=0; i<height; ++i){
		for (int j=0; j<width; ++j){
			// 计算三维坐标点: 以center_point为图像几何中心点
			//x = center_point.x + (j - center_point.x) * row_vector[0] + (i - center_point.y) * col_vector[0];
			//y = center_point.y + (j - center_point.x) * row_vector[1] + (i - center_point.y) * col_vector[1];
			//z = center_point.z + (j - center_point.x) * row_vector[2] + (i - center_point.y) * col_vector[2];

			x = center_point.x + (j - (float)(width / 2.0)) * row_vector[0] + (i - (float)(height / 2.0)) * col_vector[0];
			y = center_point.y + (j - (float)(width / 2.0)) * row_vector[1] + (i - (float)(height / 2.0)) * col_vector[1];
			z = center_point.z + (j - (float)(width / 2.0)) * row_vector[2] + (i - (float)(height / 2.0)) * col_vector[2];

			short val = -1024;
			short val_max = -1024;
			
			if (box->IsPointInBox(x, y, z)){
				TrilinearInterpolation(x, y, z, val);
				*(ptr + i * width + j) = val;
			}			
		}
	}

	//Timer::end("MPR::DoRender::Loop");

	//Timer::end("MPR::DoRender");
	//CGLogger::Info(Timer::summery());
}

void MPRRenderer::BufferTransform()
{
	if (output_vtk_image_data_ && show_buffer_){

		//vtkSmartPointer<vtkImageData> formatted_image_data;
		//vtkTransform *imageTransform = vtkTransform::New();
		//imageTransform->PostMultiply();
		///// Apply scale factor
		//float scale_factor = render_param_->GetScale();
		//imageTransform->Scale(scale_factor, scale_factor, scale_factor);
		///// Apply translation
		//float move_vector[3];
		//render_param_->GetMove(move_vector);
		//imageTransform->Translate(move_vector[0], move_vector[1], move_vector[2]);
		//vtkTransformFilter *transform_filter = vtkTransformFilter::New(); 
		//transform_filter->SetTransform(imageTransform);
		//transform_filter->SetInputData(output_vtk_image_data_);
		//transform_filter->Update();
		//formatted_image_data = (vtkImageData *)transform_filter->GetOutput();
		//if (formatted_image_data == NULL){
		//	return;
		//}

		// 原来的方法，用来对比生成的图像
		int width = output_vtk_image_data_->GetDimensions()[0];
		int height = output_vtk_image_data_->GetDimensions()[1];

		int ww, wl;
		(dynamic_cast<MPRRenderParam *>(render_param_))->GetWindowWidthLevel(ww, wl);
		// convert to 32 bits bitmap
		vtkImageData *pTmpImageData = NULL;	
		ConvertVtkImagedataToRGBA* convert = new ConvertVtkImagedataToRGBA();
		if (false == convert->ConvertImageScalarsToRGBA(output_vtk_image_data_, &pTmpImageData, -1, ww, wl))
		{
			return;
		}

		//// 写入磁盘文件
		//vtkSmartPointer<vtkBMPWriter> writer = vtkSmartPointer<vtkBMPWriter>::New();
		//writer->SetFileName(dump_file_name_.c_str());
		//writer->SetInputData(pTmpImageData);
		//writer->Write();

		UNITDATASHOW* pdata = reinterpret_cast<UNITDATASHOW*>(pTmpImageData->GetScalarPointer());
		int number_of_components = pTmpImageData->GetNumberOfScalarComponents();

		show_buffer_->SetBufferData(pdata, width, height, number_of_components * 8);

	}
}

void MPRRenderer::SetOffScreenRendering(bool flag)
{
	is_off_screen_rendering_ = flag;
}

bool MPRRenderer::PickByImagePos(const Vector2i& in_point, Vector3d& out_point)
{
	return false;
}

bool MPRRenderer::PickByImagePos(const int& x, const int& y, Vector3d& out_point)
{
	return false;
}

bool MPRRenderer::PickBy3DPoint(const Vector3d& in_point, int& x, int& y)
{
	return false;
}

float MPRRenderer::CalculateLength(Vector3d&, Vector3d&)
{
	return 0.0;
}

void MPRRenderer::GetOrientationTags(Vector3d&, Vector3d&, Vector3d&, Vector3d&)
{

}

///
// 三线性插值
///
void MPRRenderer::TrilinearInterpolation(float ix, float iy, float iz, short& ctv)
{
	short gO,gP,gQ,gR,gS,gT,gU,gV,gA,gB,gC,gD,gE,gF; 
	int nnx,nny,nnz;
	nnx = (int)ix;
	nny = (int)iy;
	nnz = MathTool::Round(iz);
	//int width = volume_data_->GetSliceWidth();
	//int height = volume_data_->GetSliceHeight();
	int slice_number = volume_data_->GetSliceCount();
	//if (nnx < 0 || nnx > width-1 || nny < 0 || nny >height-1 || nnz < 0 || nnz > slice_number - 1) {
	//	//ctv = 0;	
	//	return;
	//}

	//int nID = nny*width+nnx;
	//int nID1 = (nny+1)*width+nnx;
	float dZ = (iz-nnz);
	// 最后一层不超出范围
	int nnz_next = nnz < slice_number-1 ? nnz+1 : nnz-1;
	gO = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx, nny, nnz)));
	gP = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx, nny, nnz_next)));
	gQ = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx+1, nny, nnz)));
	gR = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx+1, nny, nnz_next)));
	gS = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx, nny+1, nnz)));
	gT = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx, nny+1, nnz_next)));
	gU = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx+1, nny+1, nnz)));
	gV = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx+1, nny+1, nnz_next)));

	gA = (short)(dZ*(gP-gO))+gO;
	gB = (short)(dZ*(gR-gQ))+gQ;
	gC = (short)(dZ*(gT-gS))+gS;
	gD = (short)(dZ*(gV-gU))+gU;
	gE = (short)((ix-nnx)*(gB-gA))+gA;
	gF = (short)((ix-nnx)*(gD-gC))+gC;
	ctv = (short)((iy-nny)*(gF-gE))+gE;
}

void MPRRenderer::BilinearInterpolation(float ix, float iy, float iz, short& ctv, short max_ctv)
{
	short gO, gP, gR, gS, gA, gB, gC;
	int nnx,nny,nnz;
	nnx = (int)ix;
	nny = (int)iy;
	nnz = MathTool::Round(iz);
	int width = volume_data_->GetSliceWidth();
	int height = volume_data_->GetSliceHeight();
	int slice_number = volume_data_->GetSliceCount();
	// x/y方向权重
	float dx = ix - nnx;
	float dy = iy - nny;

	gO = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx, nny, nnz)));
	gP = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx+1, nny, nnz)));
	gR = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx, nny+1, nnz)));
	gS = *(reinterpret_cast<short *>(volume_data_->GetDataPointer(nnx+1, nny+1, nnz)));

	if (gO > max_ctv || gP > max_ctv || gR > max_ctv || gS > max_ctv){

		// 沿x方向线性插值
		gA = dx * gP + (1 - dx) * gO;
		gB = dx * gS + (1 - dx) * gR;
		// 沿y方向线性插值
		gC = dy * gB + (1 - dy) * gA;

		ctv = gC;
	}
	else{
		ctv = max_ctv;
	}	
}

void MPRRenderer::ComputeWorldToDisplay(Point3f& world_pos, Point3f& display_pos)
{

}

void MPRRenderer::ComputeWorldToDisplay(double x, double y, double z, double display_point[3])
{

}