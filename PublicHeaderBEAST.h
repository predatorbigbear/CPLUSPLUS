#pragma once

//公共头文件


#include <boost/predef.h>


#ifdef BOOST_OS_WINDOWS
#define _WIN32_WINNT 0x0501

#if _WIN32_WINNT <= 0x0502
#define BOOST_ASIO_DISABLE_IOCP
#define BOOST_ASIO_ENABLE_CANCELIO
#endif
#endif

#include<boost/asio.hpp>
#include<boost/thread.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include<iomanip>
#include<map>
#include<vector>
#include<string>
#include<fstream>
#include<memory>
#include<algorithm>
#include<iterator>
#include<functional>
#include<iostream>
#include<filesystem>
#include<sstream>
#include<iomanip>
#include<chrono>
#include<thread>
#include<limits>
#include<unordered_map>
#include<array>
#include<string_view>
#include<tuple>
#include<ctime>
#include<cctype>
#include<type_traits>
#include<unordered_set>
#include<mutex>

using namespace boost::property_tree;
using boost::asio::io_service;
using boost::thread_group;
using boost::system::error_code;
using boost::system::system_error;
using boost::asio::steady_timer;
using boost::asio::async_read;
using boost::asio::async_write;
using boost::asio::async_read_until;
using boost::asio::streambuf;
namespace beast = boost::beast;

namespace fs = std::filesystem;
using fs::current_path;
using std::shared_ptr;
using std::unique_ptr;
using std::string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::stringstream;
using std::equal;
using std::find;
using std::boolalpha;
using std::get;
using std::stoi;
using std::chrono::seconds;
using namespace std::string_literals;
using namespace std::chrono_literals;
using std::this_thread::sleep_for;
using std::numeric_limits;
using std::unordered_map;
using std::array;
using std::tuple;
using std::back_inserter;
using std::vector;
using std::distance;
using std::to_string;
using std::begin;
using std::end;
using std::ostream_iterator;
using std::copy;
using std::hex;
using std::ispunct;
using std::stoi;
using std::string_view;
using std::count;
using std::make_pair;
using std::get;
using std::is_array;
using std::count;
using std::all_of;
using std::isdigit;
using std::bind;
using std::placeholders::_1;
using std::logical_and;
using std::less_equal;
using std::greater_equal;
using std::pair;
using std::make_pair;
using std::islower;
using std::all_of;
using std::isupper;
using std::isdigit;
using std::find_if;
using std::equal_to;
using std::transform;
using std::unordered_set;
using std::logical_or;
using std::reference_wrapper;
using std::mutex;
using std::lock_guard;



#define CURRENT_FUNCTION cout<<BOOST_CURRENT_FUNCTION<<'\n'







static string UrlDecode(const string &source)
{
	if (!source.empty())
	{
		string result{ source };
		static vector<pair<string, char>>vec{ { "%21",'!'},{"%2A",'*'}, {"%22",'\"'}, {"%27", '\''},{"%28",'('}, {"%29",')'},{ "%3B",';'},{"%3A",':'},{"%40",'@'}, {"%26",'&' },
		{ "%3D",'='}, {"%2B",'+'},{ "%24",'$'},{"%2C",','},{"%2F",'/'},{"%3F",'?'},{"%25",'%'},{"%23",'#'},{"%5B",'['},{"%5D",']' },{"%20",' '},{"%60",'`'} };

		auto iterBegin = result.begin(), iterEnd = result.end();
		vector<pair<string, char>>::iterator iter;

		while (iterBegin != iterEnd)
		{
			iterBegin = find_if(iterBegin, iterEnd, bind(logical_or<>(), bind(equal_to<>(), std::placeholders::_1, '%'), bind(equal_to<>(), std::placeholders::_1, '+')));
			if (iterBegin != iterEnd)
			{
				if (*iterBegin == '%')
				{
					for (iter = vec.begin(); iter != vec.end(); ++iter)
					{
						if (search(iterBegin, iterEnd, iter->first.begin(), iter->first.end()) == iterBegin)
							break;
					}
					if (iter != vec.end())
					{
						*iterBegin = iter->second;
						copy(iterBegin + 3, iterEnd, iterBegin + 1);
						iterEnd -= 2;
					}
				}
				else
					*iterBegin = ' ';
				++iterBegin;
			}
		}
		result.erase(iterEnd, result.end());
		return result;
	}
	return source;
}






static void praseBody(const string_view &source, const vector<string> &vecFindStr, vector<reference_wrapper<string>> &vecDisStr,bool &success)
{
	if (!source.empty() && !vecFindStr.empty() && !vecDisStr.empty() && vecFindStr.size() == vecDisStr.size())
	{
		vector<string>::const_iterator vecFindStrBegin{ vecFindStr.begin() }, vecFindStrEnd{ vecFindStr.end() };
		vector<reference_wrapper<string>>::iterator vecDisStrBegin{ vecDisStr.begin() }, vecDisStrEnd{ vecDisStr.end() };
		string_view::const_iterator iterBegin{ source.begin() }, iterEnd{ iterBegin }, iterLast{ source.end() };

		do
		{
			iterBegin = search(iterBegin, iterLast, vecFindStrBegin->begin(), vecFindStrBegin->end());
			if (iterBegin == iterLast)
				break;

			if (distance(vecFindStrBegin, vecFindStrEnd) == 1)
			{
				vecDisStrBegin->get().assign(iterBegin + vecFindStrBegin->size(), iterLast);
				iterBegin = iterLast;
			}
			else
			{
				iterEnd = find(iterBegin + vecFindStrBegin->size(), iterLast, '&');
				if (iterEnd == iterLast)
					break;

				vecDisStrBegin->get().assign(iterBegin + vecFindStrBegin->size(), iterEnd);
				iterBegin = iterEnd;
			}

			

			++vecFindStrBegin;
			++vecDisStrBegin;

		} while (iterBegin != iterLast);
		success = vecFindStrBegin == vecFindStrEnd;
	}
}