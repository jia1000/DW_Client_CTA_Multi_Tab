/*=========================================================================

  Program:   ImagingEngine
  Module:    aabb.h
  author: 	 zhangjian
  Brief:	 使用Max-min表示法的AABB包围盒
			 使用一个右上角和左下角的点来唯一的定义一个包围体

=========================================================================*/
#pragma once

#include "data_definition.h"

class AABB
{
public:
	AABB(void);
	~AABB(void);
	///
	// 自定义的有参构造函数，参数为两个Pointf结构的AABB包围盒最小最大坐标点
	///
	AABB(Point3f pointMin, Point3f pointMax);
	///
	// 自定义的有参的构造函数，参数为AABB包围盒最小最大点的坐标
	///
	AABB(float m_Xmin,float m_Xmax,float m_Ymin,float m_Ymax,float m_Zmin,float m_Zmax);
	///
	// 从一个对象拷贝
	///
	void DeepCopy(AABB* a);
	///
	// 判断两个AABB包围盒是否相交，如果相交返回TRUE，如果不相交，返回FALSE
	///
	BOOL IsInterection(AABB* a);
	///
	// 判断一三维点是否落在某个AABB包围盒内
	///
	BOOL IsPointInBox(Point3f pt);
	BOOL IsPointInBox(float x, float y, float z);
	///
	// 求AABB包围盒的中心或者质心
	///
	Point3f GetBoxCenter();
	///
	// 判断AABB包围盒是否为空
	///
	BOOL IsOrNotEmpty();
	///
	// 得到X方向上的长度
	///
	float GetXLength();
	///
	// 得到Y方向上的长度
	///
	float GetYLength();
	///
	// 得到Z方向上的长度
	///
	float GetZLength();
	///
	// 得到AABB包围盒的体积
	///
	float GetVolume();
	///
	// 绘制AABB包围盒：不适用
	///
	void DrawBox();
	///
	// 更新顶点值
	///
	void Update(float xmin,float ymin,float zmin,float xmax,float ymax,float zmax);

private:
	//X的最小值
	float Xmin;
	//X的最大值
	float Xmax;
	//Y的最小值
	float Ymin;
	//Y的最大值
	float Ymax;
	//Z的最小值
	float Zmin;
	//Z的最大值
	float Zmax;

};
