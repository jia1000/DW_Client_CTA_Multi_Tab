/*=========================================================================

  Program:   ImagingEngine
  Module:    aabb.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "data/aabb.h"

AABB::AABB(void)
{

}

//以AABB包围盒最小最大坐标点的Point3f结构体作为参数的构造函数
AABB::AABB(Point3f pointMin, Point3f pointMax)
{
	Xmin=pointMin.x;
	Ymin=pointMin.y;
	Zmin=pointMin.z;
	Xmax=pointMax.x;
	Ymax=pointMax.y;
	Zmax=pointMax.z;

}

//以AABB包围盒最小最大坐标点的六个坐标作为参数的构造函数
AABB::AABB(float m_Xmin,float m_Ymin,float m_Zmin,float m_Xmax,float m_Ymax,float m_Zmax)
{
	Xmin=m_Xmin;
	Xmax=m_Xmax;
	Ymin=m_Ymin;
	Ymax=m_Ymax;
	Zmin=m_Zmin;
	Zmax=m_Zmax;
}

AABB::~AABB(void)
{

}

/*-----IsInterection(AABB *a)---------
函数功能：判断两个AABB包围盒是否相交
函数参数：a为AABB对象指针,本身对象为b包围盒
--------------------------------------------------*/
//判断两个AABB包围盒是否相交
BOOL AABB::IsInterection(AABB *a)
{
	//如果a包围盒最小的X坐标大于b包围盒的最大X坐标,a包围盒在X方向上在b的右边
	if (a->Xmin>Xmax){
		return FALSE;
	}
	else if (a->Xmax<Xmin){
		//如果a包围盒最大的X坐标小于b包围盒的最小的X坐标，a包围盒在X方向上在b的左边
		return FALSE;
	}
	else if (a->Ymin>Ymax){
		//如果a包围盒最小的Y坐标大于b包围盒的最大Y坐标，a包围盒在Y方向上在b的右边
		return FALSE;
	}
	else if (a->Ymax<Ymin){
		//如果a包围盒最大的Y坐标小于b包围盒的最小的Y坐标,a包围盒在Y方向上在b的左边
		return FALSE;
	}
	else if (a->Zmin>Zmax){
		//如果a包围盒最小的Z坐标大于b包围盒的最大Z坐标，a包围盒在Z方向上在b的上方
		return FALSE;
	}
	else if (a->Zmax<Zmin){
		//如果a包围盒最大的Z坐标小于b包围盒的最小的Z坐标,a包围盒在Z方向上在b的下方
		return FALSE;
	}
	else{
		return TRUE;
	}
}

/*-----IsPointInBox判断点是否落在AABB包围盒之内------
函数功能：判断三维点是否落在AABB包围盒之内
函数参数：pt为Point3f结构体，box为AABB包围盒对象指针
---------------------------------------------------------------------------------------------*/
BOOL AABB::IsPointInBox(Point3f pt)
{
	return (pt.x>=Xmin)&&(pt.x<=Xmax)&&
		(pt.y>=Ymin)&&(pt.y<=Ymax)&&
		(pt.z>=Zmin)&&(pt.z<=Zmax);
}
BOOL AABB::IsPointInBox(float x, float y, float z)
{
	return (x>=Xmin)&&(x<=Xmax)&&
		(y>=Ymin)&&(y<=Ymax)&&
		(z>=Zmin)&&(z<=Zmax);
}
/*-----GetBoxCenter()-------------------------------------------
函数功能：得到此AABB包围盒对象的质心，中心，返回一个Point3f结构体
-------------------------------------------------------------------*/
Point3f AABB::GetBoxCenter()
{
	Point3f m_point;
	m_point.x=(Xmin+Xmax)*0.5f;
	m_point.y=(Ymin+Ymax)*0.5f;
	m_point.z=(Zmin+Zmax)*0.5f;
	return m_point;
}

/*-----IsOrNotEmpty()---------------------------------------------
函数功能：判断AABB包围盒是否为空，为空返回TRUE，不为空返回FALSE
------------------------------------------------------------------*/
BOOL AABB::IsOrNotEmpty()
{
	return(Xmin>Xmax)||(Ymin>Ymax)||(Zmin>Zmax);
}

/*-----GetXLength()---------
函数功能：得到X方向上的长度
--------------------------*/
float AABB::GetXLength()
{
	return Xmax-Xmin;
}

/*-----GetYLength()---------
函数功能：得到Y方向上的长度
--------------------------*/
float AABB::GetYLength()
{
	return Ymax-Ymin;
}

/*-----GetZLength()---------
函数功能：得到Z方向上的长度
--------------------------*/
float AABB::GetZLength()
{
	return Zmax-Zmin;
}

/*-----GetVolume()-------------
函数功能：得到AABB包围盒的体积
------------------------------*/
float AABB::GetVolume()
{
	return (Xmax-Xmin)*(Ymax-Ymin)*(Zmax-Zmin);
}
/*-----DeepCopy()-------------
函数功能：拷贝顶点
------------------------------*/
void AABB::DeepCopy(AABB *a)
{
	Xmin=a->Xmin;
	Xmax=a->Xmax;
	Ymin=a->Ymin;
	Ymax=a->Ymax;
	Zmin=a->Zmin;
	Zmax=a->Zmax;
}
/*-----Update()-------------
函数功能：更新顶点
------------------------------*/
void AABB::Update(float xmin,float ymin,float zmin,float xmax,float ymax,float zmax)
{
	Xmin=xmin;
	Xmax=xmax;
	Ymin=ymin;
	Ymax=ymax;
	Zmin=zmin;
	Zmax=zmax;
}

/*-----DrawAABBBoundingBox()-------------------
函数功能：绘制AABB包围盒
-----------------------------------------------*/
void AABB::DrawBox()
{
	////第一根线
	//glBegin(GL_LINE);
	//glVertex3f(Xmin,Ymin,Zmin);
	//glVertex3f(Xmax,Ymin,Zmin);
	//glEnd();
	////第二根线
	//glBegin(GL_LINE);
	//glVertex3f(Xmin,Ymin,Zmin);
	//glVertex3f(Xmin,Ymin,Zmax);
	//glEnd();
	////第三根线
	//glBegin(GL_LINE);
	//glVertex3f(Xmin,Ymin,Zmin);
	//glVertex3f(Xmin,Ymax,Zmin);
	//glEnd();
	////第四根线
	//glBegin(GL_LINE);
	//glVertex3f(Xmin,Ymin,Zmax);
	//glVertex3f(Xmax,Ymin,Zmax);
	//glEnd();
	////第五根线
	//glBegin(GL_LINE);
	//glVertex3f(Xmax,Ymin,Zmin);
	//glVertex3f(Xmax,Ymin,Zmax);
	//glEnd();
	////第六根线
	//glBegin(GL_LINE);
	//glVertex3f(Xmax,Ymin,Zmin);
	//glVertex3f(Xmax,Ymax,Zmin);
	//glEnd();
	////第七根线
	//glBegin(GL_LINE);
	//glVertex3f(Xmax,Ymax,Zmin);
	//glVertex3f(Xmin,Ymax,Zmin);
	//glEnd();
	////第八根线
	//glBegin(GL_LINE);
	//glVertex3f(Xmin,Ymax,Zmin);
	//glVertex3f(Xmin,Ymax,Zmax);
	//glEnd();
	////第九根线
	//glBegin(GL_LINE);
	//glVertex3f(Xmax,Ymax,Zmin);
	//glVertex3f(Xmax,Ymax,Zmax);
	//glEnd();
	////第十根线
	//glBegin(GL_LINE);
	//glVertex3f(Xmin,Ymin,Zmax);
	//glVertex3f(Xmin,Ymax,Zmax);
	//glEnd();
	////第十一根线
	//glBegin(GL_LINE);
	//glVertex3f(Xmax,Ymin,Zmax);
	//glVertex3f(Xmax,Ymax,Zmax);
	//glEnd();
	////第十二根线
	//glBegin(GL_LINE);
	//glVertex3f(Xmin,Ymax,Zmax);
	//glVertex3f(Xmax,Ymax,Zmax);
	//glEnd();
}