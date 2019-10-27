#pragma once

#include "GeneralNetworkPublicHeader.h"



struct GeneralNetworkPublicMessage                                         //公共消息类，借助信号槽绑定任意地方，实现任意操作
{
	GeneralNetworkPublicMessage(shared_ptr<io_context> ioc)
	{
		p_strand.reset(new io_context::strand(*ioc));
	}

	shared_ptr<io_context::strand>p_strand;

	shared_ptr<list<string>> p_writeBuf;

	shared_ptr<boost::signals2::signal<void()>> p_Readsignal{new boost::signals2::signal<void()> ()};

	shared_ptr<boost::signals2::signal<void()>> p_Writesignal{new boost::signals2::signal<void()>() };

};








