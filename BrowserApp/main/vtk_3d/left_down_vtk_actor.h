/*
 *  
 *  $Id: LeftDownVtkActor.h $
 *  DeepWise 
 *
 *  Copyright 2017-2030 DeepWise All rights reserved.
 *  http://www.deepwise.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *  Code based in VTK
 */
#pragma once
#ifdef __DEPRECATED
#undef __DEPRECATED
#endif 

#include "global_include.h"

class LeftDownVtkActor : public vtkActor2D {
public:
	static LeftDownVtkActor *New();
	
protected:
	LeftDownVtkActor();
	~LeftDownVtkActor();
	
	vtkTypeMacro(LeftDownVtkActor, vtkActor2D);	
	
public:
	
};
