#pragma once

#include "GeneralNetworkPublicHeader.h"


struct GeneralNetworkService
{
protected:
	
	virtual void initReadBuf() = 0;                                                   //初始化读取缓冲区

	virtual void initWriteBuf() = 0;                                                  //初始化写入缓冲区

	virtual void bindReadSignal()=0;                                                  //绑定检查读取操作的信号

	virtual void bindWriteSignal() = 0;                                               //绑定检查写入操作的信号

	virtual void resetTimer() = 0;                                                    //初始化定时器

	virtual void checkTimeOut()=0;                                                    //检查超时函数

	virtual void TimeOut()=0;                                                         //超时处理函数

	virtual void startRead()=0;

	virtual void handlestartRead(const error_code &err,size_t ReadByte) = 0;          //成功则调用外界传入的函数进行处理

	virtual void startWrite() = 0;

	virtual void handlestartWrite(const error_code &err, size_t WriteByte) = 0;

	
	virtual void clean()=0;

};