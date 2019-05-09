/*=========================================================================

  Program:   ImagingEngine
  Module:    camera.h
  author: 	 zhangjian
  Brief:	 相机类，封装了vtkCamera以及相机操作接口
			 参考：https://blog.csdn.net/wzheng92/article/details/79935059

=========================================================================*/
#pragma once

#include <vtkCamera.h>
#include <vtkPlane.h>
#include <vtkSmartPointer.h>
#include <vtkMatrix4x4.h>
#include "data_definition.h"
#include "vector3.h"
#include "render_mode.h"

using namespace std;

namespace DW {
	namespace IMAGE{
		
		///
		// 相机类，封装了vtkCamera以及相机操作接口
		// 参考：https://blog.csdn.net/wzheng92/article/details/79935059
		///		
		class Camera
		{
		public:
			Camera();
			~Camera();
			/// 绕着相机向上方向旋转
			void Rotate(float angle);
			/// 绕着指定轴旋转
			void RotateWXYZ(float angle, float x, float y, float z);
			/// 绕着平面点旋转
			void Rotate(float x, float y, float angle);
			/// Zooms in on the scene with the given factor
			/// @param zoom Factor factor that we want to apply to the camera
			void Zoom(float factor);
			/// 获取vtkCamera
			vtkSmartPointer<vtkCamera> GetVtkCamera();
			/// 设置vtkCamera
			void SetVtkCamera(vtkSmartPointer<vtkCamera> camera);
			/// 深拷贝对象
			void DeepCopy(Camera*);
			/// 设置相机位置和方向
			void SetCameraViewPlane(OrientationType ori);

		private:
			// vtk相机类
			vtkCamera* vtk_camera_;
			// 相机成像平面
			vtkSmartPointer<vtkPlane> view_plane_;
			// 相机焦点位置，即成像平面中心点
			Point3f focal_point_;
			// 光心的位置
			Point3f position_;
			// 成像平面中心点: 成像的一个offset偏移量
			// 如果理想情况，图像成像中心（Focal Point）就在窗口的中心，那么WindowCenter就是(0, 0)
			// WindowCenter的正方向是向右向上的。而成像平面的坐标系往往是向右向下的
			// 平移的对象是图像，而不是在三维空间移动相机
			Point3f window_center_;
			// 图像的正方向
			Vector3f view_up_;
			// 视角，默认是30°
			// 它决定了图像中内容的比例大小，通过设置这一变量可以实现图像的放缩
			float view_angle_;
			// 如果为True那么按照平行投影进行渲染，否则默认是按照透视投影PerspectiveProjection进行渲染
			bool parallel_projection_;
			// 从光心位置Position指向焦点位置FocalPoint
			Vector3f direction_of_projection_;
			// 投影面（成像面）的法向量。与direction_of_projection_矢量正好相反
			Vector3f view_plane_normal_;
			// 焦距
			float distance_;
			// 相机矩阵的逆矩阵
			vtkSmartPointer<vtkMatrix4x4> view_transform_;
			// 应用于相机矩阵的变换矩阵
			vtkSmartPointer<vtkMatrix4x4> camera_light_transform_;

		};
	}
}