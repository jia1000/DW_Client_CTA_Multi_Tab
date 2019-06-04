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

//#include "main/controllers/streaming/streamingloader.h"


#include "studycontextmy.h"

GNC::GCS::StudyContextMy::StudyContextMy()
{
}

GNC::GCS::StudyContextMy::~StudyContextMy()
{
}

#include "api/studycontextmy.h"
#include "main/controllers/dicommanager.h"
#include "api/dicom/dicomdataset.h"
#include "api/dicom/dcmdictionary.h"

//#include "dcmtk/config/osconfig.h"
//#include "dcmtk/dcmdata/dctk.h"
//#include "dcmtk/dcmimage/diargimg.h"
//#include "dcmtk/dcmdata/dcfilefo.h"
//#include "dcmtk/dcmdata/dcdeftag.h"
//#include "dcmtk/dcmdata/dcdict.h"

void GNC::GCS::StudyContextMy::ReadDicomFile(const std::string path_file)
{
	//GNC::GCS::Ptr<GNC::GCS::IStreamingLoader> Loader  = new GNC::StreamingLoader();
	//Loader->SetInput(path_file);

	//int dimensions[3] = {0,0,0};
	//Loader->GetOutputDimensions(dimensions);

	const std::string src_path_file("C:/ztest2/10.dcm");
	const std::string dst_path_file("C:/ztest2/20.dcm");

	GIL::DICOM::DicomDataset base;
	GIL::DICOM::IDICOMManager*	pDICOMManager = new GIL::DICOM::DICOMManager();
	if(pDICOMManager) 
	{
		pDICOMManager->CargarFichero(src_path_file, base);
		std::string str_tag("");

		base.getTag(GKDCM_PatientName , str_tag);
		printf("patient name : %s(use DicomManager)\n", str_tag.c_str());
		// modify one tag
		GIL::DICOM::DicomDataset modify_base;	
		modify_base.tags["0010|0010"] = "test 105";
		pDICOMManager->ActualizarJerarquia(modify_base);

		// save dicom file
		pDICOMManager->AlmacenarFichero(dst_path_file);
		delete pDICOMManager;
		pDICOMManager = NULL;
	}
}