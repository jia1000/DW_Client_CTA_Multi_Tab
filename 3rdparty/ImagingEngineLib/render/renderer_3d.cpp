
#include "renderer_3d.h"

using namespace DW::Render;

void ThreeDRenderer::SetData(VolData* data)
{
	volume_data_ = data;
}

void ThreeDRenderer::SetRenderParam(RenderParam* param)
{
	render_param_ = param;
}

vtkSmartPointer<vtkImageData> ThreeDRenderer::GetShowBuffer()
{
	return show_buffer_;
}

void ThreeDRenderer::Render()
{

}