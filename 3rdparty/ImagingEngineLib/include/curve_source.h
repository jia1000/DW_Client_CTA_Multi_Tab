/*=========================================================================

  Program:   ImagingEngine
  Module:    curve_source.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "data/curve_line.h"

using namespace DW::IMAGE;

namespace DW {
	
	class CurveSource
	{
	public:
		~CurveSource() { instance_ = nullptr; }
	
		static CurveSource *Get();

		/// Get curve by id
		VolCurve *GetCurve(string series_uid, string id);
		/// Add a curve
		string CreateCurve(string series_uid, vector<Point3f> points);
		/// Destroy a curve
		void Destroy(string series_uid, string id);
		/// Destroy curves in a series
		void DestroyAll(string series_uid);
		/// Destroy all curves
		void DestroyAll();

	private:
		CurveSource() {}

	private:
		static CurveSource *instance_;
		/// Store curves in all series
		map<string, vector<VolCurve *>> curve_list_; 
	};

}
