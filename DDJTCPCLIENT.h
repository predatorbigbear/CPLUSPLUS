#pragma once


#include "GeneralNetworkPublicheader.h"
#include "GeneralNetworkPublicMessage.h"



template<typename READBUF = string, typename WRITEBUF = shared_ptr<list<string>>, typename ASYNCTIMER = shared_ptr<steady_timer>, typename READSIGNAL = shared_ptr<boost::signals2::signal<void()>>,
	typename WRITESIGNAL = shared_ptr<boost::signals2::signal<void()>>, typename WRITETEMPBUF = shared_ptr<string_view> ,
	typename READFUN = function<void(const string&, unsigned int, unsigned int&, string &,READSIGNAL, bool&)> >
struct DDJTCPClient
{
	DDJTCPClient(shared_ptr<io_context> context, const string &serverAddress,                                                 //客户端类
		shared_ptr<unordered_map<string, shared_ptr<GeneralNetworkPublicMessage>>> publicMessageMap,
		shared_ptr<io_context::strand> publicMessageMapStrand ,const string &firstMessage, READFUN readFun);


	void firstSend();

	void resetEndpoint();

	void resetSocket();

	void connectSocket();

	void setWiteBuf();

	void clean();



	void startRead();


	void handlestartRead(const error_code &err, size_t ReadByte);

	void checkReadBuf(size_t ReadByte);                                                                    //在这里进行检查读取缓冲区的操作




	void initAferReadSignal();

	void insertMessage(const string &sendMessage);

	void initafterWriteTobuf();

	void checkWriteBuffer();

	void startWrite();

	void handlestartWrite(const error_code &err, size_t WriteByte);

	void initReadBuf();

private:
	shared_ptr<io_context>m_context;


	string m_serverAddress;

	shared_ptr<boost::asio::ip::tcp::endpoint>m_endpoint;

	shared_ptr<boost::asio::ip::tcp::socket>m_socket;


	READBUF m_readBuf;

	const unsigned int m_maxReadSize{ maxReadSize };


	unsigned int m_readLen{};

	unsigned int m_handledReadLen{};
	

	
	ASYNCTIMER m_timer;

	atomic<bool>m_disOnline;

	shared_ptr<unordered_map<string, shared_ptr<GeneralNetworkPublicMessage>>>m_publicMessageMap;

	shared_ptr<io_context::strand>m_publicMessageMapStrand;


	WRITEBUF m_writeBuf;

	shared_ptr<io_context::strand>m_WriteBufstrand;


	string m_ClientName;

	string m_firstMessage;

	string m_sendMessage;

	READSIGNAL m_afterRead;

	WRITESIGNAL m_afterWriteTobuf;

	atomic<bool>m_canSend;

	READFUN m_readFun;

	bool m_disonlineBool{ false };

	WRITETEMPBUF m_writeTempBuf;
};

template<typename READBUF, typename WRITEBUF, typename ASYNCTIMER, typename READSIGNAL, typename WRITESIGNAL, typename WRITETEMPBUF ,typename READFUN>
inline DDJTCPClient<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF, READFUN >::DDJTCPClient(shared_ptr<io_context> context, const string & serverAddress,
	shared_ptr<unordered_map<string, shared_ptr<GeneralNetworkPublicMessage>>> publicMessageMap,
	shared_ptr<io_context::strand> publicMessageMapStrand , const string &firstMessage , READFUN readFun)
	:m_context(context), m_serverAddress(serverAddress), m_firstMessage(firstMessage)
{
	m_disOnline.store(false);
	m_canSend.store(true);
	initAferReadSignal();
	initafterWriteTobuf();
	initReadBuf();

	m_readFun = readFun;
	if (m_context)
	{
		m_publicMessageMap = publicMessageMap;
		m_publicMessageMapStrand = publicMessageMapStrand;
		m_timer.reset(new steady_timer(*m_context));

		if (!serverAddress.empty())
		{
			m_serverAddress = serverAddress;
			m_serverAddress.erase(remove_if(m_serverAddress.begin(), m_serverAddress.end(), bind(equal_to<>(), std::placeholders::_1, ' ')), m_serverAddress.end());
			

			resetEndpoint();
		}

	}
	else
		delete this;
}

template<typename READBUF, typename WRITEBUF, typename ASYNCTIMER, typename READSIGNAL, typename WRITESIGNAL, typename WRITETEMPBUF , typename READFUN>
inline void DDJTCPClient<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF , READFUN>::firstSend()
{
	if (!m_firstMessage.empty())
	{
		if constexpr (is_same<decltype(m_firstMessage), string>::value)
		{
			async_write(*m_socket, boost::asio::buffer(m_firstMessage), [this](const boost::system::error_code err, std::size_t size)
				{
					if (err)
					{

						return;
					}
				

				
				});



		}
	}


}

template<typename READBUF, typename WRITEBUF, typename ASYNCTIMER, typename READSIGNAL, typename WRITESIGNAL, typename WRITETEMPBUF , typename READFUN>
inline void DDJTCPClient<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF , READFUN>::resetEndpoint()
{
	bool success{ false };
	do
	{
		if (m_serverAddress.empty())
			break;

		auto iter = find(m_serverAddress.begin(), m_serverAddress.end(), ':');

		if (iter == m_serverAddress.end())
			break;

		error_code err;
		boost::asio::ip::address address = boost::asio::ip::address::from_string(string(m_serverAddress.begin(), iter), err);

		if (err)
			break;

		if (!all_of(iter + 1, m_serverAddress.end(), bind(logical_and<>(), bind(greater_equal<>(), std::placeholders::_1, '0'), bind(less_equal<>(), std::placeholders::_1, '9'))))
			break;

		m_endpoint.reset(new boost::asio::ip::tcp::endpoint(address, stoi(string(iter + 1, m_serverAddress.end()))));

		success = true;
	
		break;

	} while (0);
	if (success)
	{
		resetSocket();
	}
	else
	{
		resetEndpoint();
	}

}

template<typename READBUF, typename WRITEBUF, typename ASYNCTIMER, typename READSIGNAL, typename WRITESIGNAL, typename WRITETEMPBUF , typename READFUN>
inline void DDJTCPClient<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF , READFUN>::resetSocket()
{
	bool success{ false };
	do
	{
		m_socket.reset(new boost::asio::ip::tcp::socket(*m_context));

		error_code err;

		m_socket->open(m_endpoint->protocol(), err);

		if (err)
			break;

		success = true;
		break;
	} while (0);
	if (success)
	{
		connectSocket();
	}
	else
	{
		resetEndpoint();
	}

}

template<typename READBUF, typename WRITEBUF, typename ASYNCTIMER, typename READSIGNAL, typename WRITESIGNAL, typename WRITETEMPBUF , typename READFUN>
inline void DDJTCPClient<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF , READFUN>::connectSocket()
{
	m_socket->async_connect(*m_endpoint, [this](const boost::system::error_code &err)
		{
			if (err)
			{

				resetEndpoint();
				return;
			}

			m_disOnline.store(false);
			setWiteBuf();
		});

}

template<typename READBUF, typename WRITEBUF, typename ASYNCTIMER, typename READSIGNAL, typename WRITESIGNAL, typename WRITETEMPBUF , typename READFUN>
inline void DDJTCPClient<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF , READFUN>::setWiteBuf()
{
	if (m_ClientName.empty())
	{
		m_ClientName = m_socket->local_endpoint().address().to_string() + ":" + to_string(m_socket->local_endpoint().port())+"Client";
		m_publicMessageMapStrand->post([this]()
			{
				if (!m_publicMessageMap->empty())
				{
					auto iter = m_publicMessageMap->find(m_ClientName);
					if (iter == m_publicMessageMap->end())
					{
						m_publicMessageMap->insert(make_pair(m_ClientName, new GeneralNetworkPublicMessage(m_context)));

						if constexpr (is_same<decltype(iter->second->p_writeBuf), shared_ptr<list<string>>>::value)
						{
							iter->second->p_writeBuf.reset(new list<string>());
							m_writeBuf = iter->second->p_writeBuf;
							m_WriteBufstrand = iter->second->p_strand;

							cout << "yes\n";
						}

					}
				}
				else
				{
					m_publicMessageMap->insert(make_pair(m_ClientName, new GeneralNetworkPublicMessage(m_context)));

					auto iter = m_publicMessageMap->find(m_ClientName);
					if (iter != m_publicMessageMap->end())
					{
						if constexpr (is_same<decltype(iter->second->p_writeBuf), shared_ptr<list<string>>>::value)
						{				
								iter->second->p_writeBuf.reset(new list<string>());
								m_writeBuf = iter->second->p_writeBuf;
								m_WriteBufstrand = iter->second->p_strand;

						}
					}
				}



				startRead();
				insertMessage(m_firstMessage);

			});
	}
	else
	{
		startRead();
	}
}



template<typename READBUF, typename WRITEBUF, typename ASYNCTIMER, typename READSIGNAL, typename WRITESIGNAL, typename WRITETEMPBUF , typename READFUN>
inline void DDJTCPClient<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF , READFUN>::clean()
{
	if (m_socket)
	{
		error_code err;
		m_socket->cancel(err);

		m_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, err);

		m_socket->close();

	}
	resetEndpoint();
}

template<typename READBUF, typename WRITEBUF, typename ASYNCTIMER, typename READSIGNAL, typename WRITESIGNAL, typename WRITETEMPBUF , typename READFUN>
inline void DDJTCPClient<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF , READFUN>::startRead()
{
	if (!m_sendMessage.empty())
	{
		string sendTemp{ m_sendMessage };
		m_sendMessage.clear();
		insertMessage(sendTemp);
	}
	if constexpr (is_same<READBUF, string>::value)
	{
		if (!m_disOnline.load())
		{
			m_readLen -= m_handledReadLen;
			m_socket->async_read_some(boost::asio::buffer(&m_readBuf[0], m_maxReadSize - m_readLen), [this](const boost::system::error_code &err, std::size_t readBytes)
				{
					handlestartRead(err, readBytes);

				});
		}
	}
}

template<typename READBUF, typename WRITEBUF, typename ASYNCTIMER, typename READSIGNAL, typename WRITESIGNAL, typename WRITETEMPBUF , typename READFUN>
inline void DDJTCPClient<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF , READFUN>::handlestartRead(const error_code & err, size_t ReadByte)
{
	if (err)                 //错误处理
	{
		cout << err.value() << " " << err.message() << '\n';
		if (!m_disOnline.load())
		{
			m_disOnline.store(true);
			clean();
		}
		return;
	}

	//调用外部引入的函数处理读取数据

	m_readLen += ReadByte;
	checkReadBuf(ReadByte);
}


template<typename READBUF, typename WRITEBUF, typename ASYNCTIMER, typename READSIGNAL, typename WRITESIGNAL, typename WRITETEMPBUF , typename READFUN>
inline void DDJTCPClient<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF , READFUN>::checkReadBuf(size_t ReadByte)
{
	if (ReadByte)
	{
		m_readFun(m_readBuf, ReadByte, m_handledReadLen, m_sendMessage, m_afterRead, m_disonlineBool);
	}
	else
	{
		startRead();

	}
}

template<typename READBUF, typename WRITEBUF, typename ASYNCTIMER, typename READSIGNAL, typename WRITESIGNAL, typename WRITETEMPBUF , typename READFUN>
inline void DDJTCPClient<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF , READFUN>::initAferReadSignal()
{
	if constexpr (is_same<WRITESIGNAL, shared_ptr<boost::signals2::signal<void()>>>::value)
	{
		m_afterRead.reset(new boost::signals2::signal<void()>);
		m_afterRead->connect(std::bind(&DDJTCPClient::startRead, this));

	}
	else
	{
		clean();
	}
}

template<typename READBUF, typename WRITEBUF, typename ASYNCTIMER, typename READSIGNAL, typename WRITESIGNAL, typename WRITETEMPBUF , typename READFUN>
inline void DDJTCPClient<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF , READFUN>::insertMessage(const string & sendMessage)
{
	if constexpr (is_same<WRITEBUF, shared_ptr<list<string>>>::value)
	{
		if (!m_disOnline.load())
		{
			m_WriteBufstrand->post([this, sendMessage]()
				{
					m_writeBuf->emplace_back(sendMessage);
					(*m_afterWriteTobuf)();
				});
		}
	}
	else
	{
		if (!m_disOnline.load())
		{
			m_disOnline.store(true);
			clean();
		}
	}
}


template<typename READBUF, typename WRITEBUF, typename ASYNCTIMER, typename READSIGNAL, typename WRITESIGNAL, typename WRITETEMPBUF , typename READFUN>
inline void DDJTCPClient<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF , READFUN>::initafterWriteTobuf()
{
	if constexpr (is_same<READSIGNAL, shared_ptr<boost::signals2::signal<void()>>>::value)
	{
		m_afterWriteTobuf.reset(new boost::signals2::signal<void()>);
		m_afterWriteTobuf->connect(std::bind(&DDJTCPClient::checkWriteBuffer, this));


	}
	else
	{
		clean();
	}
}



template<typename READBUF, typename WRITEBUF, typename ASYNCTIMER, typename READSIGNAL, typename WRITESIGNAL, typename WRITETEMPBUF , typename READFUN>
inline void DDJTCPClient<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF , READFUN>::checkWriteBuffer()
{
	if (!m_disOnline.load())
	{
		if constexpr (is_same<WRITEBUF, shared_ptr<list<string>>>::value)
		{
			m_WriteBufstrand->post([this]()
				{
					if constexpr (is_same<WRITETEMPBUF, shared_ptr<string_view>>::value)
					{
						if (!m_writeBuf->empty() && m_canSend.load())
						{
							m_canSend.store(false);
							m_writeTempBuf.reset(new string_view(m_writeBuf->front()));
							startWrite();
						}

					}
					else
					{
						m_canSend.store(true);
					}
				});
		}


	}
}

template<typename READBUF, typename WRITEBUF, typename ASYNCTIMER, typename READSIGNAL, typename WRITESIGNAL, typename WRITETEMPBUF , typename READFUN>
inline void DDJTCPClient<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF , READFUN>::startWrite()
{
	if constexpr (is_same<WRITETEMPBUF, shared_ptr<string_view>>::value)
	{
		if (!m_disOnline.load())
		{
			async_write(*m_socket, boost::asio::buffer(*m_writeTempBuf), [this](const boost::system::error_code &err, std::size_t sizeWrite)
				{
					handlestartWrite(err, sizeWrite);
				});
		}
	}
	else
	{
		if (!m_disOnline.load())
		{
			m_disOnline.store(true);
			clean();
		}
	}
}

template<typename READBUF, typename WRITEBUF, typename ASYNCTIMER, typename READSIGNAL, typename WRITESIGNAL, typename WRITETEMPBUF , typename READFUN>
inline void DDJTCPClient<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF, READFUN>::handlestartWrite(const error_code & err, size_t WriteByte)
{
	if (err)
	{
		if (!m_disOnline.load())
		{
			m_disOnline.store(true);
			clean();
		}

		return;
	}


	m_WriteBufstrand->post([this]()
		{
			if constexpr (is_same<WRITEBUF, shared_ptr<list<string>>>::value)
			{
				if (!m_writeBuf->empty())
				{
					m_writeBuf->pop_front();
					m_canSend.store(true);
					checkWriteBuffer();
				}
				else
				{
					m_canSend.store(true);
				}
			}

		});
}

template<typename READBUF, typename WRITEBUF, typename ASYNCTIMER, typename READSIGNAL, typename WRITESIGNAL, typename WRITETEMPBUF, typename READFUN>
inline void DDJTCPClient<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF, READFUN>::initReadBuf()
{
	if constexpr (is_same<READBUF, string>::value)
	{
		m_readBuf.resize(m_maxReadSize);
	}


	else
	{
		clean();
	}
}