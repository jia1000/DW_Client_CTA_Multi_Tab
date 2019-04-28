#include "imaging_cpr.h"
#include "renderer_cpr.h"
#include "vector3.h"

using namespace DW::Render;

CPRImaging::CPRImaging()
{
	show_buffer_ = new ShowBuffer();
	show_buffer_->InitBufferData(512, 512, 32);
}

// 获取原始三维图像数据场
VolData* CPRImaging::GetData()
{
	return volume_data_;
}
// 设置原始三维图像数据场
void CPRImaging::SetData(VolData* data)
{
	volume_data_ = data;
}
// 获取输出显示图像
ShowBuffer* CPRImaging::GetShowBuffer()
{
	if (renderer_){
		show_buffer_ = renderer_->GetShowBuffer();
	}
	return show_buffer_;
}
// 渲染图像，缓存到show_buffer_
void CPRImaging::Render()
{
	if (renderer_){
		renderer_->Render();
	}
}

void CPRImaging::SetRenderer(IThreedRenderer* renderer)
{
	renderer_ = renderer;
}