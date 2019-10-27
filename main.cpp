
#include "GeneralNetWorkEasyEnjoy.h"

#include "DDJTCPSERVER.h"
#include "DDJTCPService.h"


template<typename READBUF = string, typename READSIGNAL = shared_ptr<boost::signals2::signal<void()>>>
struct testServerRead
{
	static void readFun(const READBUF &readBuf, unsigned int readLen, unsigned int &handleLen, string &writeStr,
	     READSIGNAL afterRead, bool &disOnline)
	{
		if constexpr (is_same<READBUF, string>::value &&
			is_same<READSIGNAL, shared_ptr<boost::signals2::signal<void()>>>::value)
		{
			if (readLen)
			{
				string sendMessage{ readBuf.begin(), readBuf.begin() + readLen };
				cout << "received:"<< sendMessage<<'\n';
				writeStr= sendMessage;
			}


			handleLen = readLen;
			(*afterRead)();		                     //接收到数据之后将接收到的信息作为返回信息，然后触发信号
		}
		else
		{
			disOnline = true;
		}
	}


};



int main()
{
	
	
	function<void(const string&, unsigned int, unsigned int&, string &writeStr,
		shared_ptr<boost::signals2::signal<void()>>, bool&)>f1{ &testServerRead<>::readFun };


	GeneralNetWorkEasyEnjoy<> gnwe;

	gnwe.setServer("127.0.0.1:8090", 300 , f1);                                //外层传入操作函数，

	gnwe.setClient("127.0.0.1:8090","First SendMessage is this.",f1);


	gnwe.Enjoy();
	

	return 0;
}


