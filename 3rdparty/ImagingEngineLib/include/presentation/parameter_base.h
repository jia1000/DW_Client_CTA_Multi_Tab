/*=========================================================================

  Program:   ImagingEngine
  Module:    parameter_base.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

namespace DW {

	class RenderParameter
	{
	public:

	protected:
		/// 图像显示宽高
		int display_width_;
		int display_height_;
		/// 图像窗宽窗位
		int window_width_;
		int window_level_;
		/// 图像放缩比例
		float scale_factor_;
		/// (平移后)图像显示的中心点
		float display_center_;
		/// 图像中心点
		float image_center_;
		/// 曲线/中心线ID
		int curve_id_;
		/// MIP = 1/MinIP = 2/AIP = 3
		int blend_mode_;
		/// 插值方法: Cubic = 1/Linear = 2/NearestNeighbor = 3
		int interpolation_mode_;
		/// 层厚
		float thickness_;
		/// VR是否填充整个窗口
		bool fit_window_;

	private:
	};

}
