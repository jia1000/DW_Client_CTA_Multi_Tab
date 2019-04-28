/*=========================================================================

  Program:   ImagingEngine
  Module:    data_definition.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

// Windows 头文件: 
#include <windows.h>
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <stdint.h>

#define UNITDATA3D			unsigned char
#define UNITMARK3D			unsigned char
#define UNITDATASHOW		char

typedef enum dwRenderMode {
	Slice = 0,
	Slab,
	MPR,
	VR,
	CPR
} RenderMode;

struct Point3d {
	float x;
	float y;
	float z;
};

struct Point3f {
	float x;
	float y;
	float z;
};

struct Point3i {
	int x;
	int y;
	int z;
};

struct Point2 {
	int x;
	int y;
};
