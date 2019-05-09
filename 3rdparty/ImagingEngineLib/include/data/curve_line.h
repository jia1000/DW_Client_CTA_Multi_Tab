/*=========================================================================

  Program:   ImagingEngine
  Module:    curve_line.h
  author: 	 zhangjian
  Brief:	 三维曲线类，通过cardinal样条插值生成采样点

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "data/vector2.h"
#include "data/vector3.h"

using namespace std;

namespace DW {
	namespace IMAGE {

		class VolCurve
		{
		public:
			VolCurve();
			~VolCurve();

			/// 获取曲线编号
			string GetCurveID() {return curve_id_;}
			/// 添加一个三维控制点
			virtual void AddControlPoint(Point3f);
			virtual void AddControlPoint(float x, float y, float z);
			/// 移除一个三维控制点
			virtual void RemoveControlPoint(int);
			/// 返回指定位置的控制点坐标
			virtual void GetControlPoint(int pos, float& x, float& y, float& z);
			/// 返回指定位置的采样点坐标
			virtual void GetSamplePoint(int pos, float& x, float& y, float& z);
			/// 进行采样
			virtual void Update();
			/// 控制点数目
			virtual int GetNumberOfControlPoint()
			{
				return control_points_.size();
			}
			/// 采样点数目
			virtual int GetNumberOfSamplePoint()
			{
				return sample_points_.size();
			}
			/// 设置采样间隔
			virtual void SetSamplingInterval(float interval)
			{
				sampling_interval_ = interval;
			}
			/// 返回采样间隔
			virtual float GetSamplingInterval()
			{
				return sampling_interval_;
			}
			/// 计算两点之间近似长度（单位：像素）
			virtual float CalculateDistance(int from_index, int to_index);
			/// 获取曲线方向
			virtual void GetCurveDirection(float& x, float& y, float& z);
			/// 获取采样点的切线方向向量
			void GetTangentVector(int index, Vector3f& tangent);
			/// 获取采样点局部平面的方向向量
			void GetLocalPlaneVector(int index, Vector3f& x, Vector3f& y);

		protected:
			/// 判断是否是同一个点
			bool IsSame3DPoint(Point3f ControlPoint);
			/// 实现生成采样点
			void GenerateCurve();
			/// 计算采样点的切线/法线/副法线向量
			void ComputeSampleVectors();
			/// 计算一个切线向量
			void ComputeTangentVector(Point3f last_pnt, Point3f next_pnt, Vector3f& tangent);
			/// 计算一个法线向量
			void ComputeNormalVector(Vector3f last_tangent, Vector3f next_tangent, Vector3f& normal);
			/// 根据关联性计算一个法线向量
			void ComputeConsistentNormalVector(Vector3f tangent, Vector3f last_normal, Vector3f& normal);

		protected:
			/// 曲线编号
			string curve_id_;
			/// 三维数据场控制点列表
			vector<Point3f> control_points_;
			/// 三维数据场采样点列表
			vector<Point3f> sample_points_;
			/// 采样间隔
			float sampling_interval_;
			/// 每个采样点的切线向量
			vector<Vector3f> tangent_vectors_;
			/// 每个采样点的法线向量
			vector<Vector3f> normal_vectors_;
			/// 每个采样点的副法线向量
			vector<Vector3f> binormal_vectors_;
			/// 法向量之间关联性
			bool using_consistent_normal_;
		};

	}
}