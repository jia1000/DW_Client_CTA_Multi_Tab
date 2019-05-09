/*
 *  
 *  $Id: ilock.cpp $
 *  DeepWise Dicom Viewer
 *
 *  Copyright 2017-2030 DeepWise All rights reserved.
 *  http://www.deepwise.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */

#include "main/controllers/streaming/streamingloader.h"


#include "studycontextmy.h"

GNC::GCS::StudyContextMy::StudyContextMy()
{
}

GNC::GCS::StudyContextMy::~StudyContextMy()
{
}


void GNC::GCS::StudyContextMy::ReadDicomFile(const std::string path_file)
{
	GNC::GCS::Ptr<GNC::GCS::IStreamingLoader> Loader  = new GNC::StreamingLoader();
	Loader->SetInput(path_file);

	int dimensions[3] = {0,0,0};
	Loader->GetOutputDimensions(dimensions);
}