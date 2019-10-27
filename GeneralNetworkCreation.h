#pragma once

#include "GeneralNetworkPublicHeader.h"


struct GeneralNetwork
{
	GeneralNetwork();

	void setreadMethod(const function<string(const string&)> &readDataMethod);                 //�趨��ȡ�Ĳ�������

	void setwriteMethod(const function<string(const string&)> &WriteDataMethod);               //�趨д��Ĳ�������






	


private:
	string m_IPAddress;       //IP��ַ
	
	unsigned int m_Port{ numeric_limits<unsigned int>::max() };      //�����˿�

	unsigned int m_CreationNetWorkType{ numeric_limits<unsigned int>::max() };                  //��������tcp udp
 
	unsigned int m_CreationIPType{ numeric_limits<unsigned int>::max() };                       //��������ipv4 ����ipv6

	function<string(const string&)>m_readDataMethod;                                            //���ݵĶ�ȡ�����ӿ�

	function<string(const string&)>m_writeDataMethod;                                           //���ݵ�д������ӿ�





	void setCreationNetWorkType();                                                             //�趨��������    

	void setCreationIPType();                                                                  //�趨������IP����

	void setIPAddress();                                                                       //�趨IP��ַ

	void setPort();                                                                            //�趨�˿ں�

	
};