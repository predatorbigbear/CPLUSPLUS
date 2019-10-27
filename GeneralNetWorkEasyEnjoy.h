#pragma once


#include "GeneralNetworkPublicHeader.h"
#include "GeneralNetworkCore.h"
#include "GeneralNetworkCreation.h"
#include "DDJTCPService.h"


template<typename READBUF = string, typename WRITEBUF = shared_ptr<list<string>>, typename ASYNCTIMER = shared_ptr<steady_timer>, typename READSIGNAL = shared_ptr<boost::signals2::signal<void()>>,
	typename WRITESIGNAL = shared_ptr<boost::signals2::signal<void()>>, typename WRITETEMPBUF = shared_ptr<string_view>,
typename READFUN= function<void(const string&, unsigned int, unsigned int&, string &,
	READSIGNAL, bool&)>  >
class GeneralNetWorkEasyEnjoy
{

public:
	GeneralNetWorkEasyEnjoy() = default;

	GeneralNetWorkEasyEnjoy operator= (const GeneralNetWorkEasyEnjoy<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL,
		WRITESIGNAL, WRITETEMPBUF, READFUN> &temp) = delete;

	GeneralNetWorkEasyEnjoy(const GeneralNetWorkEasyEnjoy<READBUF, WRITEBUF, ASYNCTIMER, READSIGNAL,
		WRITESIGNAL, WRITETEMPBUF, READFUN> &temp) = delete;




	void setServer(const string &bindAddress , unsigned int heartTime , READFUN readFun)
	{
		m_GeneralNetWorkCore->setServer(bindAddress , heartTime , readFun);

	}


	void setClient(const string &serverAddress, const string &firstMessage, READFUN readFun)
	{
		m_GeneralNetWorkCore->setClient(serverAddress, firstMessage , readFun);
	}






	void Enjoy()
	{
		m_GeneralNetWorkCore->EnjoyGeneralNetWork();
	}




private:
	unique_ptr<GeneralNetWorkCore<READBUF ,WRITEBUF ,ASYNCTIMER , READSIGNAL,
		WRITESIGNAL,WRITETEMPBUF, READFUN>>m_GeneralNetWorkCore
	{new GeneralNetWorkCore<READBUF,WRITEBUF,ASYNCTIMER,READSIGNAL,WRITESIGNAL,WRITETEMPBUF,READFUN>() };

};


