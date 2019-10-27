#pragma once

#include "GeneralNetworkPublicheader.h"
#include "GeneralNetWorkClassType.h"
#include "GeneralNetworkPublicMessage.h"
#include "DDJTCPService.h"



template<typename READBUF = string, typename WRITEBUF = shared_ptr<list<string>>, typename ASYNCTIMER = shared_ptr<steady_timer>, typename READSIGNAL = shared_ptr<boost::signals2::signal<void()>>,
	typename WRITESIGNAL = shared_ptr<boost::signals2::signal<void()>>, typename WRITETEMPBUF = shared_ptr<string_view>,
	template<typename READBUF = string, typename WRITEBUF = shared_ptr<list<string>>, typename ASYNCTIMER = shared_ptr<steady_timer>, typename READSIGNAL = boost::signals2::signal<void()>,
	typename WRITESIGNAL = boost::signals2::signal<void()>, typename WRITETEMPBUF = shared_ptr<string_view>>
	class TCPSERVICE = DDJTCPService,
	typename READFUN = function<void(const string&, unsigned int, unsigned int&,string &,
		READSIGNAL, bool&)>  >
struct DDJTCPSERVER :public classType<ClassType::MyTCPServer>
{
	
	DDJTCPSERVER(shared_ptr<io_context> context, const string bindAddress,
		shared_ptr<unordered_map<string, shared_ptr<GeneralNetworkPublicMessage>>> publicMessageMap,
		shared_ptr<io_context::strand>publicMessageMapStrand , unsigned int heartTime ,
		READFUN readFun)
		:m_context(context), m_publicMessageMap(publicMessageMap), m_publicMessageMapStrand(publicMessageMapStrand), m_heartTime(heartTime)
	{


		m_readFun = readFun;
		if (m_context)    //有效
		{

			if (!bindAddress.empty())
			{
				string bindAddressTemp{ bindAddress };
				bindAddressTemp.erase(remove_if(bindAddressTemp.begin(), bindAddressTemp.end(), bind(equal_to<>(), std::placeholders::_1, ' ')), bindAddressTemp.end());

				if (!bindAddressTemp.empty())
				{

					auto iter = find(bindAddressTemp.begin(), bindAddressTemp.end(), ':');
					if (iter != bindAddressTemp.end())
					{
						m_bingAddress.assign(bindAddressTemp.begin(), iter);
						if (all_of(iter + 1, bindAddressTemp.end(), bind(logical_and<>(), bind(greater_equal<>(), std::placeholders::_1, '0'), bind(less_equal<>(), std::placeholders::_1, '9'))))
						{
							m_bindPort = stoi(string(iter + 1, bindAddressTemp.end()));
							if (m_bindPort && m_bindPort <= maxPort)
							{

								cout << "start server " << bindAddressTemp << "\n";
								start();
							}

							//start to next

						}
					}
				}
			}
		}
		else              //无效
		{

		}
	}



	void setMap()
	{
		

	}
	

	
	void start()
	{
		resetEndpoint();
	}


	
	void resetEndpoint()
	{
		error_code err;
		boost::asio::ip::address ad{ boost::asio::ip::address::from_string(m_bingAddress,err) };
		if (err)
		{
			cout << m_bingAddress << " is not aninvaild address\n"
				<< err.value() << ":" << err.message() << '\n';
			start();
		}
		else
		{
			m_endpoint.reset(new boost::asio::ip::tcp::endpoint(ad, m_bindPort));
			resetAcceptor();
		}
	}



	
	void resetAcceptor()
	{
		m_acceptor.reset(new boost::asio::ip::tcp::acceptor(*m_context, m_endpoint->protocol()));
		bindAcceptor();

	}




	void bindAcceptor()
	{
		error_code err;
		m_acceptor->bind(*m_endpoint, err);
		if (err)
		{
			cout << m_bingAddress << " is not aninvaild address\n"
				<< err.value() << ":" << err.message() << '\n';

			start();
		}
		else
		{
			listenAcceptor();
		}
	}



	void listenAcceptor()
	{
		error_code err;
		m_acceptor->listen(numeric_limits<unsigned short>::max(), err);
		if (err)
		{
			cout << "bind \n";

			start();
		}
		else
		{

			accept();
		}
	}



	
	void accept()
	{
		shared_ptr<boost::asio::ip::tcp::socket>socket{ new boost::asio::ip::tcp::socket(*m_context) };
		m_acceptor->async_accept(*socket, [this, socket](const boost::system::error_code &err)
		{
			handleAccept(err, socket);

		});
	}




	void handleAccept(const error_code & err, shared_ptr<boost::asio::ip::tcp::socket> socket)
	{
		if (err)
		{
			cout << err.value() << " " << err.message() << '\n';
			start();
		}
		else
		{
			string ServiceName = socket->remote_endpoint().address().to_string() + ":" + to_string(socket->remote_endpoint().port());
			cout << socket->remote_endpoint().address().to_string() << ":" << socket->remote_endpoint().port() << '\n';

			auto iter = m_publicMessageMap->find(ServiceName);
			if (iter != m_publicMessageMap->end())
			{
				iter->second.reset(new GeneralNetworkPublicMessage(m_context));
			}
			else
			{
				m_publicMessageMap->insert(make_pair(ServiceName, new GeneralNetworkPublicMessage(m_context)));
			}

			(new DDJTCPService<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF , READFUN>(m_context, socket, m_publicMessageMap,
				m_publicMessageMapStrand, ServiceName , m_heartTime, m_readFun));
			
			accept();
		}
	}







private:
	shared_ptr<io_context>m_context;
	unique_ptr<boost::asio::ip::tcp::acceptor>m_acceptor;
	unique_ptr<boost::asio::ip::tcp::endpoint>m_endpoint;

	string m_bingAddress;
	unsigned int m_bindPort;

	unsigned int m_heartTime;
	


	shared_ptr<unordered_map<string, shared_ptr<GeneralNetworkPublicMessage>>>m_publicMessageMap;

	shared_ptr<io_context::strand>m_publicMessageMapStrand;

	READFUN m_readFun;
};

