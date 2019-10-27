#pragma once

enum ClassType { MyTCPServer, MyTCPClient };             //指定tcpServer类型


template<size_t COMMUNICATIONTYPE>
struct classType
{
	static constexpr size_t baseType = COMMUNICATIONTYPE;
};








