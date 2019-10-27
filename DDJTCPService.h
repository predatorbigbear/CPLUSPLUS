#pragma once


#include "GeneralNetworkPublicheader.h"
#include "GeneralNetworkService.h"
#include "GeneralNetworkPublicMessage.h"



template<typename READBUF=string,typename WRITEBUF=shared_ptr<list<string>>,typename ASYNCTIMER=shared_ptr<steady_timer>, typename READSIGNAL= shared_ptr<boost::signals2::signal<void()>>, 
	typename WRITESIGNAL= shared_ptr<boost::signals2::signal<void()>>, typename WRITETEMPBUF=shared_ptr<string_view>, 
	typename READFUN = function<void(const string&, unsigned int, unsigned int&, string&,
		READSIGNAL, bool&)>  >
struct DDJTCPService:public GeneralNetworkService                                            //服务器实例类
{
	DDJTCPService() =default;

	DDJTCPService(shared_ptr<io_context> context, shared_ptr <boost::asio::ip::tcp::socket> socket,
		shared_ptr<unordered_map<string, shared_ptr<GeneralNetworkPublicMessage>>> publicMessageMap,
		shared_ptr<io_context::strand> publicMessageMapStrand, const string &ServiceName ,unsigned int heartTime ,
		READFUN readFun):
		m_context(context), m_socket(socket), m_publicMessageMap(publicMessageMap), m_publicMessageMapStrand(publicMessageMapStrand), m_ServiceName(ServiceName)
	{
		m_readFun = readFun;

		
		m_maxReadSize = maxReadSize;
		m_maxWriteSize = maxWriteSize;

		m_canSend.store(true);
		m_disOnline.store(false);

		initReadBuf();
		initWriteBuf();
		

		bindWriteBufAndStrand();

		initAferReadSignal();
		initafterWriteTobuf();

		cout << "Service\n";


		if (heartTime)
		{
			m_heartTime = heartTime;
			resetTimer();
		}
		
		start();
		
	}



	void start()
	{
		startRead();

	}


	void initAferReadSignal()
	{
		if constexpr (is_same<WRITESIGNAL, shared_ptr<boost::signals2::signal<void()>>>::value)
		{
			m_afterRead.reset(new boost::signals2::signal<void()>);
			m_afterRead->connect(std::bind(&DDJTCPService::startRead, this));

		}
		else
		{
			clean();
		}
	}


	void initafterWriteTobuf()
	{
		if constexpr (is_same<READSIGNAL, shared_ptr<boost::signals2::signal<void()>>>::value)
		{
			m_afterWriteTobuf.reset(new boost::signals2::signal<void()>);
			m_afterWriteTobuf->connect(std::bind(&DDJTCPService::checkWriteBuffer, this));


		}
		else
		{
			clean();
		}
	}






	void bindWriteBufAndStrand()                                      //绑定写入缓冲区和写入缓冲区strand
	{
		if (m_publicMessageMapStrand && m_publicMessageMap)
		{
			if (!m_ServiceName.empty())
			{
				m_publicMessageMapStrand->post([this]()
				{
					auto iter = m_publicMessageMap->find(m_ServiceName);
					if (iter != m_publicMessageMap->end())
					{
						if constexpr (is_same<WRITEBUF, shared_ptr<list<string>>>::value)
						{
							iter->second->p_writeBuf.reset(new list<string>());
							m_writeBuf = iter->second->p_writeBuf;
						}
						else
						{
							clean();
						}

						iter->second->p_Writesignal->connect(bind(&DDJTCPService::startCheckWriteBuffer, this));

						m_WriteBufstrand = iter->second->p_strand;
					}	
					else   
					{
						clean();                 //无效

					}
				});
			}
			else
			{
				clean();               //无效
				
			}
		}
		else
		{
			clean();
		}
	}






	void resetTimer()override                                        //初始化和重置定时器
	{
		if (m_context)
		{
			if constexpr (is_same<ASYNCTIMER, decltype(m_heartTimer)>::value)
			{
				m_heartTimer.reset(new steady_timer(*m_context));

				checkTimeOut();
			}
			else
			{
				clean();
			}


		}

	}





	void checkTimeOut()override
	{
		if constexpr (is_same<ASYNCTIMER, shared_ptr<steady_timer>>::value)
		{
			if (!m_disOnline.load())
			{
				m_heartTimer->expires_from_now(seconds(m_heartTime));
				m_heartTimer->async_wait([this](const boost::system::error_code &err)
				{
						m_err = err;

						TimeOut();
				});
			}
			else
			{
				clean();
			}

		}
	}





	void cancelTimer()
	{
		if constexpr (is_same<ASYNCTIMER, shared_ptr<steady_timer>>::value)
		{
			m_heartTimer->cancel();
		}


	}






	void TimeOut()override                                                            //   检查定时器结束之后的执行
	{
		if (m_err)
		{
			if (m_err == boost::asio::error::operation_aborted)                      //    需要检查是否置位结束
			{
				checkTimeOut();

			}
			else
			{

				clean();
			}
			return;
		}

		clean();
	}







	void initReadBuf()override                                                                                  //初始化读取缓冲区
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








	void initWriteBuf()override
	{
		if constexpr (is_same<WRITEBUF, shared_ptr<list<string>> >::value)
		{
			m_writeBuf.reset(new list<string>());
		}


		else
		{
			clean();
		}
	}








	void startRead()override
	{
		if (m_disonlineBool)
		{
			m_disOnline.store(true);
		}
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






	void handlestartRead(const error_code &err, size_t ReadByte)override                  //接收到读取数据之后
	{
		if (err)                 //错误处理
		{
			m_disOnline.store(true);
			return;
		}

		//调用外部引入的函数处理读取数据

		m_readLen += ReadByte;
		cancelTimer();

		checkReadBuf(ReadByte);
	}




	void checkReadBuf(size_t ReadByte)                                                                    //在这里进行检查读取缓冲区的操作
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

	
	



	void startCheckWriteBuffer()
	{
		if (m_canSend.load())
		{
			m_canSend.store(false);
			checkWriteBuffer();
		}



	}





	void startWrite()override
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
			m_disOnline.store(true);
		}


	}








	void handlestartWrite(const error_code &err, size_t WriteByte)override
	{
		if (err)
		{
			m_disOnline.store(true);


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









	void checkWriteBuffer()                                                           //检查写入缓冲区
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














	void clean()override
	{
		m_disOnline.store(true);
		error_code err;
		m_socket->cancel(err);
		m_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, err);
		m_socket->close(err);
		m_socket.reset();

		m_publicMessageMapStrand->post([this]()
			{
				auto iter = m_publicMessageMap->find(m_ServiceName);
				if (iter != m_publicMessageMap->end())
				{
					m_publicMessageMap->erase(iter);
				}

				cout << m_ServiceName << " delete service\n";
				delete this;
			});
	}





	void bindReadSignal()
	{

	}

	void bindWriteSignal()
	{

	}


	void insertPublicMessage(const string &sendToWho,const string &sendMessage)
	{
		if (!sendToWho.empty() && !sendMessage.empty())
		{
			m_publicMessageMapStrand->post([this, sendToWho, sendMessage]()
				{
					auto iter = m_publicMessageMap->find(sendToWho);
					if (iter != m_publicMessageMap->end())
					{
						iter->second->p_strand->post([this, iter, sendMessage]()
							{
								iter->second->p_writeBuf->emplace_back(sendMessage);
								(*iter->second->p_Writesignal)();
							});
					}
				});

		}
	}


	void insertMessage(const string &sendMessage)
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
			m_disOnline.store(true);
		}


	}



private:
	shared_ptr<io_context>m_context;

	shared_ptr <boost::asio::ip::tcp::socket> m_socket;

	READBUF m_readBuf;

	unsigned int m_readLen{};

	unsigned int m_handledReadLen{};

	unsigned int m_maxReadSize{};

	unsigned int m_maxWriteSize{};

	shared_ptr<unordered_map<string, shared_ptr<GeneralNetworkPublicMessage>>>m_publicMessageMap;

	shared_ptr<io_context::strand>m_publicMessageMapStrand;

	WRITETEMPBUF m_writeTempBuf;


	

	
	
	WRITEBUF m_writeBuf;

	shared_ptr<io_context::strand>m_WriteBufstrand;


	string m_ServiceName;

	bool m_disonlineBool{ false };

	error_code m_err;

	atomic<bool>m_disOnline;

	atomic<bool>m_canSend;

	unsigned int m_heartTime{};          //心跳时间

	ASYNCTIMER m_heartTimer;

	WRITETEMPBUF sendToWho;

	WRITETEMPBUF sendMessage;

	READSIGNAL m_afterRead;

	WRITESIGNAL m_afterWriteTobuf;

	string m_sendMessage;

	READFUN m_readFun;
};




