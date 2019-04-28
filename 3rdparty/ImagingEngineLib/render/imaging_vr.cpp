#include "imaging_vr.h"
#include "renderer_raycasting_cpu.h"
#include "vector3.h"

using namespace DW::Render;

VRImaging::VRImaging()
{
	show_buffer_ = new ShowBuffer();
	show_buffer_->InitBufferData(512, 512, 32);
}

// 获取原始三维图像数据场
VolData* VRImaging::GetData()
{
	return volume_data_;
}
// 设置原始三维图像数据场
void VRImaging::SetData(VolData* data)
{
	volume_data_ = data;
}
// 获取输出显示图像
ShowBuffer* VRImaging::GetShowBuffer()
{
	if (renderer_){
		show_buffer_ = renderer_->GetShowBuffer();
		//if (vtk_image_data){
		//	//原来的方法，用来对比生成的图像
		//	int width = vtk_image_data->GetDimensions()[0];
		//	int height = vtk_image_data->GetDimensions()[1];
	
		//	// 从8位转换到32位
		//	int slice = -1;

		//	UNITDATA3D* pdata = reinterpret_cast<UNITDATA3D*>( vtk_image_data->GetScalarPointer() );
		//	int number_of_components = vtk_image_data->GetNumberOfScalarComponents();
		//	
		//	show_buffer_->SetBufferData(pdata, width, height, number_of_components * 8);
		//}
	}
	return show_buffer_;
}
// 渲染图像，缓存到show_buffer_
void VRImaging::Render()
{
	if (renderer_){
		renderer_->Render();
	}
}
// 放缩
void VRImaging::Zoom(int scale)
{

}
// 移动
void VRImaging::Move(int distance) 
{

}
// 平面内旋转
void VRImaging::Rotate(float angle)
{

}
// 绕着中心轴旋一定角度
void VRImaging::Rotate3D(Vector3d axis, float angle) 
{

}
// 窗宽窗位
void VRImaging::WindowWidthLevel(int width, int level) 
{

}

void VRImaging::SetRenderer(IThreedRenderer* renderer)
{
	renderer_ = renderer;
}