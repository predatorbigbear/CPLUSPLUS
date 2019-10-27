#pragma once

#include "GeneralNetworkPublicHeader.h"


struct GeneralNetwork
{
	GeneralNetwork();

	void setreadMethod(const function<string(const string&)> &readDataMethod);                 //设定读取的操作方法

	void setwriteMethod(const function<string(const string&)> &WriteDataMethod);               //设定写入的操作方法






	


private:
	string m_IPAddress;       //IP地址
	
	unsigned int m_Port{ numeric_limits<unsigned int>::max() };      //启动端口

	unsigned int m_CreationNetWorkType{ numeric_limits<unsigned int>::max() };                  //创建的是tcp udp
 
	unsigned int m_CreationIPType{ numeric_limits<unsigned int>::max() };                       //创建的是ipv4 还是ipv6

	function<string(const string&)>m_readDataMethod;                                            //传递的读取操作接口

	function<string(const string&)>m_writeDataMethod;                                           //传递的写入操作接口





	void setCreationNetWorkType();                                                             //设定创建类型    

	void setCreationIPType();                                                                  //设定创建的IP类型

	void setIPAddress();                                                                       //设定IP地址

	void setPort();                                                                            //设定端口号

	
};