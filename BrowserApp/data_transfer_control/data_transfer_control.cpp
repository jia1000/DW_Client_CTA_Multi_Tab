#include "stdafx.h"

#include "data_transfer_control.h"

DataTransferController* DataTransferController::instance = nullptr;

DataTransferController* DataTransferController::GetInstance()
{
	if (instance == nullptr)
		instance = new DataTransferController();
	return instance;
}

DataTransferController::DataTransferController()
{
}

DataTransferController::~DataTransferController()
{
	instance = nullptr;
}
