/*=========================================================================

  Program:   ImagingEngine
  Module:    batch.h
  author: 	 zhangjian
  Brief:	 生成批处理

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "show_buffer.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Batch{
		
		class BatchArgs;
		class IBatchCallback;

		///
		// 生成批处理基类
		///
		class IBatchGenerator
		{
		public:
			virtual void Execute(BatchArgs*) = 0;
			virtual void SetCallback(IBatchCallback*) = 0;
			virtual void Destroy() = 0;
			ShowBuffer** GetResults() {return buffer_data_set_;}
		protected:
			ShowBuffer** buffer_data_set_;
			IBatchCallback* callback_;
		};

		///
		// 批处理参数基类
		///
		class BatchArgs
		{
		public:

		protected:

		};

		class IBatchCallback
		{
		public:
			virtual void OnBtachGenerated(ShowBuffer** results, BYTE status) = 0;
		};
	}
}