#pragma once
/*
 *  
 *  $Id: ilock.h $
 *  DeepWise Dicom Viewer
 *
 *  Copyright 2017-2030 DeepWise All rights reserved.
 *  http://www.deepwise.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#ifndef STUDY_CONTEXT_MY_H
#define STUDY_CONTEXT_MY_H
//---------------------------------------------------------------------------------

#include <string>


namespace GNC {
	namespace GCS {
		class StudyContextMy
		{
		public:
			StudyContextMy();
			~StudyContextMy();

			void ReadDicomFile(const std::string path_file);
		private:

		};

		
		
	}
}

#endif