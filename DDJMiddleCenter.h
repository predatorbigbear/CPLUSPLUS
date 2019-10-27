#pragma once




#include "GeneralNetworkPublicHeader.h"
#include "GeneralNetworkPublicMessage.h"
#include "DDJTCPSERVER.h"
#include "DDJTCPService.h"
#include "DDJTCPCLIENT.h"

template<typename READBUF = string, typename WRITEBUF = shared_ptr<list<string>>, typename ASYNCTIMER = shared_ptr<steady_timer>, typename READSIGNAL = boost::signals2::signal<void()>,
	typename WRITESIGNAL = boost::signals2::signal<void()>, typename WRITETEMPBUF = shared_ptr<string_view>,
	
	typename READFUN = function<void(const string&, unsigned int, unsigned int&, string &,
		READSIGNAL, bool&)> >
struct DDJMiddleCenter
{
	DDJMiddleCenter(shared_ptr<io_context> context) :m_context(context) 
	{
		cout << "Middle Center start\n";

		m_publicMessageMapStrand.reset(new io_context::strand(*m_context));
	}
	

	DDJMiddleCenter(DDJMiddleCenter<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, READFUN> &temp) = delete;


	DDJMiddleCenter operator=(DDJMiddleCenter<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, READFUN> &temp) = delete;


	
	void setServer(const string &bindAddress, unsigned int heartTime , READFUN readFun)
	{
		new DDJTCPSERVER<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF, DDJTCPService, READFUN>(m_context, bindAddress,
			m_publicMessageMap, m_publicMessageMapStrand , heartTime , readFun);
		

	}


	void setClient(const string &serverAddress , const string &firstMessage, READFUN readFun)
	{
		new DDJTCPClient<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL, WRITESIGNAL, WRITETEMPBUF>(m_context, serverAddress, m_publicMessageMap, m_publicMessageMapStrand , firstMessage , readFun);


	}














private:
	shared_ptr<io_context> m_context;


	shared_ptr<unordered_map<string, shared_ptr<GeneralNetworkPublicMessage>>>m_publicMessageMap
	{ new unordered_map<string, shared_ptr<GeneralNetworkPublicMessage>>() };

	shared_ptr<io_context::strand>m_publicMessageMapStrand;

};