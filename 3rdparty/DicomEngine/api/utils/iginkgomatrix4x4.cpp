/*
 *  
 *  $Id: iginkgomatrix4x4.cpp $
 *  DeepWise Dicom Viewer
 *
 *  Copyright 2017-2030 DeepWise All rights reserved.
 *  http://www.deepwise.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#if defined(__DEPRECATED)
#undef __DEPRECATED
#endif
#include "iginkgomatrix4x4.h"
#include <main/utils/ginkgomatrix4x4.h>

GNC::GCS::IGinkgoMatrix4x4::IGinkgoMatrix4x4()
{
}

GNC::GCS::IGinkgoMatrix4x4::~IGinkgoMatrix4x4()
{
}

GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> GNC::GCS::IGinkgoMatrix4x4::New()
{
	GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> newPtr((GNC::GCS::IGinkgoMatrix4x4*) new GNC::GinkgoMatrix4x4());
	return newPtr;
}