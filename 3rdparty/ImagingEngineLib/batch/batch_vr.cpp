#include "batch/batch_vr.h"

using namespace DW::Batch;

VRBatchGenerator::VRBatchGenerator()
{

}

VRBatchGenerator::~VRBatchGenerator()
{

}

void VRBatchGenerator::SetCallback(IBatchCallback* cb)
{
	callback_ = cb;
}

void VRBatchGenerator::Execute(BatchArgs* args)
{
	if (callback_){
		callback_->OnBtachGenerated(buffer_data_set_, true);
	}
}

void VRBatchGenerator::Destroy()
{

}