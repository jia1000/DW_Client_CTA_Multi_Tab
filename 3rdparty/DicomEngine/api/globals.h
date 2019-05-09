/*
 *  
 *  $Id: globals.h $
 *  DeepWise Dicom Viewer
 *
 *  Copyright 2017-2030 DeepWise All rights reserved.
 *  http://www.deepwise.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#pragma once

#if 0
	#if defined (_GINKGO_DEBUG)

		#include <iostream>
		#if defined(_WINDOWS)
		#define _WINSOCKAPI_ 
		#include <windows.h>
		#endif
		
		#define GDEBUG(expr) std::cout << "D: " << expr << "   @ " << __FILE__ << ":" << __LINE__ << std::endl;

	#else // !defined (_GINKGO_DEBUG)
		 #define GDEBUG(expr) /* nada */
	#endif

	#if defined (_GINKGO_TRACE)
		 #define GTRACE(expr) std::cout << "T: " << expr << "   @ " << __FILE__ << ":" << __LINE__ << std::endl;

	#else // !defined (_GINKGO_DEBUG)
		 #define GTRACE(expr) /* nada */
	#endif
#else
	#define GTRACE(expr) /* nada */
#endif

#if defined(__WXGTK__) || defined(__WXMAC__) || defined(__WXMSW__)

#if defined (_WIN32)
// 	#define TOPATH(stringwx) std::string(stringwx.c_str())/*std::string((stringwx.To8BitData().data()==NULL)?"":stringwx.To8BitData())*/
// 	#define FROMPATH(stringstd) wxString(stringstd.c_str())
#define TOPATH(stringwx) std::string(stringwx.ToStdString())
#define FROMPATH(stringstd) wxString(stringstd.c_str())
#else
	#define TOPATH(stringwx) std::string(stringwx.ToUTF8())
	#define FROMPATH(stringstd) wxString::FromUTF8(stringstd.c_str())
#endif

#endif
