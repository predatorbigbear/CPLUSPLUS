#include<string>
#include<algorithm>
#include<iostream>
#include<functional>
#include<sstream>
#include<iomanip>

using std::boolalpha;
using std::stringstream;
using std::placeholders::_1;
using std::not_equal_to;
using std::bind;
using std::find_if;
using std::cout;
using std::find;
using std::copy;
using std::string;
using std::search;
using std::getline;
using std::logical_and;
using std::greater_equal;
using std::less_equal;
using std::logical_or;
using std::equal_to;



struct MYREQ
{
	MYREQ() = default;

	void setMethod(string::const_iterator iterBegin, string::const_iterator iterEnd)
	{
		methodStr.assign(iterBegin, iterEnd);
	}

	void setBody(string::const_iterator iterBegin, string::const_iterator iterEnd)
	{
		bodyStr.assign(iterBegin, iterEnd);
	}

	void setVersion(string::const_iterator iterBegin, string::const_iterator iterEnd)
	{
		versionStr.assign(iterBegin, iterEnd);
	}

	void setTarget(string::const_iterator iterBegin, string::const_iterator iterEnd)
	{
		targetStr.assign(iterBegin, iterEnd);
	}

	const string& method()const
	{
		return methodStr;
	}

	const string& body()const
	{
		return bodyStr;
	}

	const string& version()const
	{
		return versionStr;
	}

	const string& target()const
	{
		return targetStr;
	}

	void clear()
	{
		methodStr.clear();
		bodyStr.clear();
		versionStr.clear();
		targetStr.clear();
	}


private:
	string methodStr;
	string bodyStr;
	string versionStr;
	string targetStr;
};



void parseHttp(const string &source, MYREQ &req)
{
	if (!source.empty())
	{
		req.clear();

		string::const_iterator iterBegin{ find_if(source.begin(),source.end(),bind(not_equal_to<>(),std::placeholders::_1,' ')) },
			iterLast{ source.end() }, iterEnd, thisEnd;


		string HTTP{ "HTTP/" };
		string final{ "\r" };

		string temp;
		int num{};
		int length{};
		bool Connection{ false };
		bool allHead{false};
		bool has_length{ false };

		while (iterBegin != iterLast)
		{
			thisEnd = find(iterBegin, iterLast, '\r');

			if (thisEnd == iterLast)
				break;

			++num;
			if (num == 1)
			{
				do
				{
					iterEnd = find(iterBegin, thisEnd, ' ');
					if (iterEnd == thisEnd)
						break;
					req.setMethod(iterBegin, iterEnd);


					iterBegin = find_if(iterEnd, thisEnd, bind(not_equal_to<>(), std::placeholders::_1, ' '));
					if (iterBegin == thisEnd)
						break;
					iterEnd = find(iterBegin, thisEnd, ' ');
					if (iterEnd == thisEnd)
						break;
					req.setTarget(iterBegin, iterEnd);


					iterBegin = search(iterEnd, thisEnd, HTTP.begin(), HTTP.end());
					if (iterBegin == thisEnd)
						break;
					iterEnd = search(iterBegin + HTTP.size(), thisEnd, final.begin(), final.end());
					req.setVersion(iterBegin + HTTP.size(), iterEnd);

					break;
				} while (iterBegin != thisEnd);

				if (thisEnd != iterLast)
				{
					iterBegin = find_if(thisEnd + 1, iterLast, bind(not_equal_to<>(), std::placeholders::_1, '\n'));
					if (iterBegin == iterLast)
						break;
				}
				else
					break;
			}
			else
			{
				if (!allHead)
				{
					if (*iterBegin == '\r')
						allHead = true;
					else
					{
						do
						{
							iterEnd = find(iterBegin, thisEnd, ':');
							if (iterEnd == thisEnd)
								break;
							temp.assign(iterBegin, iterEnd);

							if (temp == "Content-Length")
							{
								iterEnd = find(iterEnd, thisEnd, ' ');
								if (iterEnd == thisEnd)
									break;
								iterBegin = find_if(iterEnd, thisEnd, bind(not_equal_to<>(), std::placeholders::_1, ' '));
								if (iterBegin == thisEnd)
									break;
								iterEnd = find(iterBegin, thisEnd, '\r');
								if (distance(iterBegin, thisEnd)>1 &&  all_of(iterBegin, iterEnd, bind(logical_and<>(), bind(greater_equal<>(), std::placeholders::_1, '0'), bind(less_equal<>(), std::placeholders::_1, '9'))))
								{
									has_length = true;
									length = stoi(string(iterBegin, iterEnd));
								}
								break;
							}
							else if (temp == "Connection")
							{
								iterEnd = find(iterEnd, thisEnd, ' ');
								if (iterEnd == thisEnd)
									break;
								iterBegin = find_if(iterEnd, thisEnd, bind(not_equal_to<>(), std::placeholders::_1, ' '));
								if (iterBegin == thisEnd)
									break;
								iterEnd = find(iterBegin, thisEnd, '\r');
								if(distance(iterBegin, thisEnd) > 1)
									temp.assign(iterBegin, iterEnd);
								if (temp == "keep-alive")
								{
									Connection = true;
								}
								break;
							}
							else
								break;
						} while (iterBegin != thisEnd);
						if (thisEnd != iterLast)
						{
							iterBegin = find_if(thisEnd + 1, iterLast, bind(not_equal_to<>(), std::placeholders::_1, '\n'));
							if (iterBegin == iterLast)
								break;
						}
						else
							break;
					}
				}
				else
				{
					if (find(iterBegin, iterLast, '=') != iterLast)
					{
						iterBegin = find_if(iterBegin, iterLast, bind(logical_and<>(), bind(not_equal_to<>(), std::placeholders::_1, '\r'), bind(not_equal_to<>(), std::placeholders::_1, '\n')));
						req.setBody(iterBegin, find(iterBegin, iterLast, '\r'));
						break;
					}
					else
					{
						if (all_of(iterBegin, iterLast, bind(logical_or<>(), bind(equal_to<>(), std::placeholders::_1, '\r'), bind(equal_to<>(), std::placeholders::_1, '\n'))))
							break;
						else
						{
							iterBegin = find_if(thisEnd, iterLast, bind(not_equal_to<>(), std::placeholders::_1, '\n'));
							if (iterBegin == iterLast)
								break;
						}
					}
				}
			}
		}

		if ((req.method() != "GET" && !has_length) || (req.method() == "GET" && !has_length && req.body().empty()) || (has_length && length == req.body().size()) )
		{

			cout << "method:" << req.method() << '\n'
				<< "body:" << req.body() << '\n'
				<< "version:" << req.version() << '\n'
				<< "target:" << req.target() << '\n'
				<< "length:" << length << '\n'
				<< "keep-alive:" << boolalpha << Connection << '\n';
		}
	}
}




int main()
{
	string str1{ "POST /assert=quertCession HTTP/1.1\r\nContent-Type: application/x-www-form-urlencoded\r\nUser-Agent: PostmanRuntime/7.17.1\r\nCache-Control: no-cache\r\nPostman-Token: 242ef077-936e-4e95-95b9-9a5aca6cf724\r\nHost: 127.0.0.1:8080\r\nAccept-Encoding: gzip, deflate\r\nContent-Length: 35\r\nConnection: keep-alive\r\n\r\nTypeNameOfRepairingEquipment=200A02\r\n" };
	
	string str2{ "GET / HTTP/1.1\r\nContent-Type: application/x-www-form-urlencoded\r\nUser-Agent: PostmanRuntime/7.17.1\r\nCache-Control: no-cache\r\nPostman-Token: 242ef077-936e-4e95-95b9-9a5aca6cf724\r\nHost: 127.0.0.1:8080\r\nAccept-Encoding: gzip, deflate\r\nConnection: close\r\n\r\n" };

	string str3{ "POST / HTTP/1.1\r\nContent-Type: application/x-www-form-urlencoded\r\nUser-Agent: PostmanRuntime/7.17.1\r\nCache-Control: no-cache\r\nPostman-Token: 242ef077-936e-4e95-95b9-9a5aca6cf724\r\nHost: 127.0.0.1:8080\r\nAccept-Encoding: gzip, deflate\r\n\r\n" };


	string str4{ "POST /assert=quertCession HTTP/1.1\r\nContent-Type: application/x-www-form-urlencoded\r\nUser-Agent: PostmanRuntime/7.17.1\r\nCache-Control: no-cache\r\nPostman-Token: 242ef077-936e-4e95-95b9-9a5aca6cf724\r\nHost: 127.0.0.1:8080\r\nAccept-Encoding: gzip, deflate\r\nContent-Length: 40\r\nConnection: keep-alive\r\n\r\nTypeNameOfRepairingEquipment=200A02\r\n" };

	string str5{ "GET / HTTP/1.1\r\nContent-Type: application/x-www-form-urlencoded\r\nUser-Agent: PostmanRuntime/7.17.1\r\nCache-Control: no-cache\r\nPostman-Token: 242ef077-936e-4e95-95b9-9a5aca6cf724\r\nHost: 127.0.0.1:8080\r\nAccept-Encoding: gzip, deflate\r\nConnection: close\r\n\r\nTypeNameOfRepairingEquipment=200A02" };

	string str6{ "GET / HTTP/1.1\r\nContent-Type: application/x-www-form-urlencoded\r" };

	string str7{ "GET / HTTP/1.1\r\nContent-Type: application/x-www-form-urlencoded" };


	MYREQ req;

	parseHttp(str1,req);

	cout << "\n\n\n";

	parseHttp(str2,req);

	cout << "\n\n\n";

	parseHttp(str3, req);

	cout << "\n\n\n";

	parseHttp(str4, req);

	cout << "\n\n\n";

	parseHttp(str5, req);

	cout << "\n\n\n";

	parseHttp(str6, req);

	cout << "\n\n\n";

	parseHttp(str7, req);

	cout << "\n\n\n";

	return 0;
}
