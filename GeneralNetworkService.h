#pragma once

#include "GeneralNetworkPublicHeader.h"


struct GeneralNetworkService
{
protected:
	
	virtual void initReadBuf() = 0;                                                   //��ʼ����ȡ������

	virtual void initWriteBuf() = 0;                                                  //��ʼ��д�뻺����

	virtual void bindReadSignal()=0;                                                  //�󶨼���ȡ�������ź�

	virtual void bindWriteSignal() = 0;                                               //�󶨼��д��������ź�

	virtual void resetTimer() = 0;                                                    //��ʼ����ʱ��

	virtual void checkTimeOut()=0;                                                    //��鳬ʱ����

	virtual void TimeOut()=0;                                                         //��ʱ������

	virtual void startRead()=0;

	virtual void handlestartRead(const error_code &err,size_t ReadByte) = 0;          //�ɹ��������紫��ĺ������д���

	virtual void startWrite() = 0;

	virtual void handlestartWrite(const error_code &err, size_t WriteByte) = 0;

	
	virtual void clean()=0;

};