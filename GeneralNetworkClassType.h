#pragma once

enum ClassType { MyTCPServer, MyTCPClient };             //ָ��tcpServer����


template<size_t COMMUNICATIONTYPE>
struct classType
{
	static constexpr size_t baseType = COMMUNICATIONTYPE;
};








