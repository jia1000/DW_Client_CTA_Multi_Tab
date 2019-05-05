
/****************************************************************************************************

 * \file data_transfer_control.h
 * \date 2019/05/05 10:33
 *
 * \author jiayf
 * Contact: @deepwise.com
 *
 * \brief : c++和js 数据交互管理 

 Copyright(c) 2018-2030 DeepWise Corporation

****************************************************************************************************/

#pragma once

class DataTransferController
{
public:
	~DataTransferController();

	static DataTransferController* GetInstance();





private:
	DataTransferController();
	static DataTransferController* instance;
};
