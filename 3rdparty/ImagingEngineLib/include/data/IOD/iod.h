/*=========================================================================

  Program:   ImagingEngine
  Module:    iod.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "data_definition.h"

using namespace DW::IMAGE;
using namespace std;

namespace DW {
	namespace IMAGE{
		///
		// BaseIOD
		///
		class BaseIOD
		{
		public:
			BaseIOD();
			virtual ~BaseIOD();
			//virtual void Read() = 0;
		};
	}
}