#include "imaging_3d.h"
#include "tools/convert_vtk_image_data_to_rgba.h"

using namespace DW::Render;

// 获取原始三维图像数据场
VolData* ThreeDImaging::GetData()
{
	return volume_data_;
}
// 设置原始三维图像数据场
void ThreeDImaging::SetData(VolData* data)
{
	volume_data_ = data;
}
// 获取输出显示图像
ShowBuffer* ThreeDImaging::GetShowBuffer()
{
	return show_buffer_;
}
// 渲染图像，缓存到show_buffer_
void ThreeDImaging::Render()
{
	
}
// 放缩
void ThreeDImaging::Zoom(int scale)
{

}
// 移动
void ThreeDImaging::Move(int distance) 
{

}
// 平面内旋转
void ThreeDImaging::Rotate(float angle)
{

}
// 绕着中心轴旋一定角度
void ThreeDImaging::Rotate3D(Vector3D axis, float angle) 
{

}
// 窗宽窗位
void ThreeDImaging::WindowWidthLevel(int width, int level) 
{

}

void ThreeDImaging::SetRenderer(ThreeDRenderer* renderer)
{
	renderer_ = renderer;
}