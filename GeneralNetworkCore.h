#pragma once

#include "GeneralNetworkPublicheader.h"
#include "DDJMiddleCenter.h"
#include "DDJTCPService.h"


template<typename READBUF = string, typename WRITEBUF = shared_ptr<list<string>>, typename ASYNCTIMER = shared_ptr<steady_timer>, typename READSIGNAL = boost::signals2::signal<void()>,
	typename WRITESIGNAL = boost::signals2::signal<void()>, typename WRITETEMPBUF = shared_ptr<string_view>,
	
	typename READFUN = function<void(const string&, unsigned int, unsigned int&,string &,
		READSIGNAL, bool&)> >
struct GeneralNetWorkCore
{
	GeneralNetWorkCore()
	{
		cout << "Welcome to GeneralNetwork,if you has any question ,"
			<< "Contact me through QQ 1075716088 .\n";
		cout << "Let's enjoy GeneralNetwork ,"
			<< "it is made by STL and BOOST .\n";
		cout << "My name is DengDanJun ,"
			<< "I will show you that STL is also great .\n";


		m_DDJMiddleCenter.reset(new DDJMiddleCenter<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL,
			WRITESIGNAL, WRITETEMPBUF , READFUN>(m_ioc));

	}


	GeneralNetWorkCore(GeneralNetWorkCore<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF,  READFUN> &temp) = delete;

	GeneralNetWorkCore operator=(GeneralNetWorkCore<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF, READFUN> &temp) = delete;

	
	void setServer(const string &bindAddress ,  unsigned int heartTime , READFUN readFun)
	{
		if (m_DDJMiddleCenter)
		{
			m_DDJMiddleCenter->setServer(bindAddress , heartTime , readFun);
		}

	}




	void setClient(const string &serverAddress, const string &firstMessage , READFUN readFun)
	{
		if (m_DDJMiddleCenter)
		{
			m_DDJMiddleCenter->setClient(serverAddress, firstMessage, readFun);
		}

	}






	void setThreadNum(unsigned int threadNum = std::thread::hardware_concurrency())
	{
		m_hasSetThreadNum = true;
		if (!threadNum)
			threadNum = std::thread::hardware_concurrency();
		if (!threadNum)
			threadNum = 8;
		for (auto startThreadNum = 0; startThreadNum != threadNum; ++startThreadNum)
		{
			m_tg.create_thread([this]()
				{
					m_ioc->run();
				});
		}
	}





	void EnjoyGeneralNetWork()
	{
		if (!m_hasEnjoy)
		{
			m_hasEnjoy = true;
			if (!m_hasSetThreadNum)
			{

				setThreadNum(std::thread::hardware_concurrency());
			}
			try
			{




				m_tg.join_all();
			}
			catch (const boost::system::system_error &err)
			{
				cout << err.code() << " " << err.what() << '\n';
			}
		}
	}




private:

	shared_ptr<io_context>m_ioc{new io_context() };
	unique_ptr<io_context::work>m_work{new io_context ::work(*m_ioc)};

	thread_group m_tg;

	unique_ptr<DDJMiddleCenter<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF, READFUN>>m_DDJMiddleCenter;


	bool m_hasSetThreadNum{ false };

	bool m_hasEnjoy{ false };

};











