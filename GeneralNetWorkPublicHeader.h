#pragma once

//公共头文件

#include<boost/asio.hpp>
#include<boost/signals2.hpp>
#include<boost/thread.hpp>
#include<boost/asio/steady_timer.hpp>
#include <boost/predef.h>





#include<iostream>
#include<memory>
#include<thread>
#include<fstream>
#include<string>
#include<filesystem>
#include<sstream>
#include<functional>
#include<atomic>
#include<queue>
#include<list>
#include<string_view>
#include<chrono>
#include<unordered_map>
#include<map>
#include<vector>
#include<sstream>
#include<fstream>
#include<filesystem>
#include<limits>
#include<functional>
#include<any>
#include<type_traits>
#include<iterator>
#include<typeinfo>



using boost::asio::steady_timer;
using boost::thread_group;
using boost::asio::io_context;
using boost::system::error_code;
using boost::asio::async_read;
using boost::asio::async_write;





using namespace std::chrono_literals;
using std::stringstream;
using std::make_shared;
using std::cout;
using std::shared_ptr;
using std::unique_ptr;
using std::string;
using std::ifstream;
namespace fs = std::experimental::filesystem;
using fs::current_path;
using fs::path;
using std::getline;
using std::search;
using std::equal;
using std::bind;
using std::placeholders::_1;
using std::equal_to;
using std::not_equal_to;
using std::find_if;
using std::all_of;
using std::greater_equal;
using std::logical_and;
using std::logical_or;
using std::less_equal;
using std::less;
using std::greater;
using std::stoi;
using std::atomic;
using std::queue;
using std::list;
using std::string_view;
using std::unordered_map;
using std::to_string;
using std::pair;
using std::vector;
using std::make_pair;
using std::stringstream;
using std::ofstream;
using std::cin;
using fs::temp_directory_path;
using std::numeric_limits;
using std::function;
using std::chrono::seconds;
using std::any;
using std::make_shared;
using std::is_same;
using std::map;
using std::hash;
using std::ostream_iterator;



#define CURRENT_FUNCTION cout<<BOOST_CURRENT_FUNCTION<<'\n'


enum popType { Messagelist , PublicMessageList};



static const int maxMessageSize{ 65535 };




static string ServerPort{ "ServerPort:" };

static string DBhostName{ "DBhostName:" };

static string DBuserName{ "DBuserName:" };

static string DBPassword{ "DBPassword:" };

static string DBName{ "DBName:" };

static string DBPort{ "DBPort:" };

static string CheckLogSecond{ "CheckLogSecond:" };




static string StartLog{ "StartLog" };

static string StartMiddle{ "StartMiddle" };

static string StartDB{ "StartDB" };






enum CreationType {TCP,UDP,IPV4,IPV6};




static unsigned int maxPort{65535};

static unsigned int maxReadSize{ 65535 };

static unsigned int maxWriteSize{ 65535 };

static unsigned int heartTime{ 30 };








