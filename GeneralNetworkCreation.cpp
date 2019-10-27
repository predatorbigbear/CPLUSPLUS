#include "GeneralNetworkCreation.h"

GeneralNetwork::GeneralNetwork()
{
	cout << "Welcome to GeneralNetwork,if you has any question ,"
		<<"add my QQ 1075716088 .\n";
	cout << "Let's enjoy GeneralNetwork ,"
		<<"it is made by STL and BOOST .\n";
	cout << "My name is DengDanJun ,"
		<<"I will show you that STL is also great .\n";


	setCreationNetWorkType();
	setCreationIPType();




}

void GeneralNetwork::setreadMethod(const function<string(const string&)>& readDataMethod)
{
	m_readDataMethod = readDataMethod;
}

void GeneralNetwork::setwriteMethod(const function<string(const string&)>& WriteDataMethod)
{
	m_readDataMethod = WriteDataMethod;
}



void GeneralNetwork::setCreationNetWorkType()
{
	do
	{
		cout << "please input Creation NetWork Type:"
			<< CreationType::TCP << " means TCP , " << CreationType::UDP << " means UDP:";
		cin >> m_CreationNetWorkType;
		cout << '\n';
	} while (m_CreationNetWorkType != CreationType::TCP && m_CreationNetWorkType != CreationType::UDP);

}



void GeneralNetwork::setCreationIPType()
{
	do
	{
		cout << "please input Creation NetWork Type:"
			<< CreationType::IPV4 << " means IPV4 , " << CreationType::IPV6 << " means IPV6:";
		cin >> m_CreationIPType;
		cout << '\n';
	} while (m_CreationIPType != CreationType::IPV4 && m_CreationNetWorkType != CreationType::IPV6);

}

void GeneralNetwork::setIPAddress()
{
	do
	{
		cout << "Please input IP Address:";
		cin >> m_IPAddress;
		cout << '\n';
		error_code err;
		boost::asio::ip::address GeneralNetworkAddress;
		GeneralNetworkAddress.from_string(m_IPAddress.c_str(),err);
		if (err)
		{
			cout << "m_IPAddress:" << m_IPAddress << '\n';
			m_IPAddress.clear();
			continue;
		}
		if (GeneralNetworkAddress.is_v4())
		{
			break;
		}
		else
		{
			m_IPAddress.clear();
			continue;
		}

	} while (m_IPAddress.empty());
}

void GeneralNetwork::setPort()
{
	do
	{
		cout << "Please input port:";
		cin >> m_Port;
		cout << '\n';
	} while (m_Port<=1 || m_Port>=65535);

}
