


#include"SQL.h"


#include<boost/current_function.hpp>

#include<boost/asio.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/strand.hpp>
#include <boost/make_unique.hpp>
#include <boost/optional.hpp>
#include <boost/config.hpp>



#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>




using namespace boost::beast::http;

using boost::asio::io_context;

namespace beast = boost::beast;                 // from <boost/beast.hpp>
namespace http = beast::http;                   // from <boost/beast/http.hpp>
namespace websocket = beast::websocket;         // from <boost/beast/websocket.hpp>
namespace net = boost::asio;                    // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;               // from <boost/asio/ip/tcp.hpp>




// Return a reasonable mime type based on the extension of a file.
beast::string_view  
mime_type(beast::string_view path)
{
	 

	using beast::iequals;
	auto const ext = [&path]
	{
		auto const pos = path.rfind(".");
		if (pos == beast::string_view::npos)
			return beast::string_view{};
		return path.substr(pos);
	}();
	if (iequals(ext, ".htm"))  return "text/html";
	if (iequals(ext, ".html")) return "text/html";
	if (iequals(ext, ".php"))  return "text/html";
	if (iequals(ext, ".css"))  return "text/css";
	if (iequals(ext, ".txt"))  return "text/plain";
	if (iequals(ext, ".js"))   return "application/javascript";
	if (iequals(ext, ".json")) return "application/json";
	if (iequals(ext, ".xml"))  return "application/xml";
	if (iequals(ext, ".swf"))  return "application/x-shockwave-flash";
	if (iequals(ext, ".flv"))  return "video/x-flv";
	if (iequals(ext, ".png"))  return "image/png";
	if (iequals(ext, ".jpe"))  return "image/jpeg";
	if (iequals(ext, ".jpeg")) return "image/jpeg";
	if (iequals(ext, ".jpg"))  return "image/jpeg";
	if (iequals(ext, ".gif"))  return "image/gif";
	if (iequals(ext, ".bmp"))  return "image/bmp";
	if (iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
	if (iequals(ext, ".tiff")) return "image/tiff";
	if (iequals(ext, ".tif"))  return "image/tiff";
	if (iequals(ext, ".svg"))  return "image/svg+xml";
	if (iequals(ext, ".svgz")) return "image/svg+xml";
	return "application/text";
}

// Append an HTTP rel-path to a local filesystem path.
// The returned path is normalized for the platform.
std::string  
path_cat(beast::string_view base,beast::string_view path)
{
	if (base.empty())
		return std::string(path);
	std::string result(base);
#ifdef BOOST_MSVC
	char constexpr path_separator = '\\';
	if (result.back() == path_separator)
		result.resize(result.size() - 1);
	result.append(path.data(), path.size());
	for (auto& c : result)
		if (c == '/')
			c = path_separator;
#else
	char constexpr path_separator = '/';
	if (result.back() == path_separator)
		result.resize(result.size() - 1);
	result.append(path.data(), path.size());
#endif
	return result;
}

// This function produces an HTTP response for the given
// request. The type of the response object depends on the
// contents of the request, so the interface requires the
// caller to pass a generic lambda for receiving the response.
template<
	class Body, class Allocator,
	class Send>
	void 
	handle_request(beast::string_view doc_root, http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send , shared_ptr<SQL> m_sql,
		shared_ptr<io_context::strand> m_strand)
{

	// Returns a bad request response
	auto const bad_request =
		[&req](beast::string_view why)
	{
		http::response<http::string_body> res{ http::status::bad_request, req.version() };
		res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		res.set(http::field::content_type, "text/html");
		res.keep_alive(req.keep_alive());
		res.body() = std::string(why);
		res.prepare_payload();
		return res;
	};


	// Returns a not found response
	auto const not_found =
		[&req](beast::string_view target)
	{
		http::response<http::string_body> res{ http::status::not_found, req.version() };
		res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		res.set(http::field::content_type, "text/html");
		res.keep_alive(req.keep_alive());
		res.body() = "The resource '" + std::string(target) + "' was not found.";
		res.prepare_payload();
		return res;
	};



	// Returns a server error response
	auto const server_error =
		[&req](beast::string_view what)
	{
		http::response<http::string_body> res{ http::status::internal_server_error, req.version() };
		res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		res.set(http::field::content_type, "text/html");
		res.keep_alive(req.keep_alive());
		res.body() = "An error occurred: '" + std::string(what) + "'";
		res.prepare_payload();
		return res;
	};


	beast::error_code ec;
	http::file_body::value_type body;


	static vector<string>interface { "/test/InsertDATA","/test/getAll" };
	string target{ req.target() }, checkStr{ req.body() };
	auto method = req.method();

	static string insertData{ "insertData=" };

	string insertDataStr;

	string::iterator iterBegin, iterEnd;

	auto vecIter = interface.begin();
	for (; vecIter != interface.end(); ++vecIter)
	{
		if (search(target.begin(), target.end(), vecIter->begin(), vecIter->end()) != target.end())
		{
			break;
		}
	}

	vector<string> vecFindStr;
	vector<reference_wrapper<string>> vecDisStr;
	bool success;


	if (vecIter != interface.end())
	{
		if (!distance(interface.begin(), vecIter))        //µÇÂ½½Ó¿Ú   /test/InsertDATA
		{
			cout << "/test/InsertDATA\n";
			vecFindStr.assign({ insertData });
			vecDisStr.assign({ insertDataStr });
			praseBody(req.body(), vecFindStr, vecDisStr, success);

			if (!success || insertDataStr.empty())
			{
				return send(bad_request("Illegal request"));
			}
			else
			{
				m_strand->post([insertDataStr, &req, m_sql, &send]
				{
					bool success{ false };
					stringstream sstr;
					string result;
					do
					{
						sstr << "insert into testTable(data) values('" << insertDataStr << "')";
						if (!m_sql->Mymysql_real_query(sstr.str()))
							break;

						success = true;
					} while (0);
					http::response<http::string_body> res{ http::status::ok, req.version() };
					res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
					res.keep_alive(req.keep_alive());



					if (success)
					{
						result = "success insert data";
						res.content_length(result.size());
						res.body() = result;
						res.prepare_payload();
					}
					else
					{
						result = "fail to insert data";
						res.content_length(result.size());
						res.body() = result;
						res.prepare_payload();
					}
					return send(std::move(res));
				});
			}
		}
		else if (distance(interface.begin(), vecIter) == 1)
		{
			cout << "/test/getAll\n";
			m_strand->post([ &req, m_sql, &send]
			{
				bool success{ false };
				stringstream sstr;
				ptree ptjson, pt1, pt2;
				string result;
				vector<vector<pair<string, string>>> resultVec;
				do
				{
					sstr << "select * from testTable ";
					if (!m_sql->Mymysql_real_query(sstr.str()))
						break;

					if (!m_sql->MysqlResult(resultVec))
						break;

					success = true;
				} while (0);
				http::response<http::string_body> res{ http::status::ok, req.version() };
				res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
				res.keep_alive(req.keep_alive());

				if (success)
				{
					for (auto const &innerVec : resultVec)
					{
						for (auto const &myPair : innerVec)
						{
							pt1.put(myPair.first, myPair.second);
						}
						pt2.push_back(make_pair("", pt1));
						pt1.clear();
					}

					ptjson.put_child("result", pt2);
					sstr.str("");
					sstr.clear();
					write_json(sstr, ptjson);
					
					res.content_length(sstr.str().size());
					res.body() = sstr.str();
					res.prepare_payload();
				}
				else
				{
					result = "fail to insert data";
					res.content_length(result.size());
					res.body() = result;
					res.prepare_payload();
				}
				return send(std::move(res));
			});

		}
	}
	else
	{

		// Make sure we can handle the method
		if (method != http::verb::post &&
			method != http::verb::get)
			return send(bad_request("Unknown HTTP-method"));


		// Request path must be absolute and not contain "..".
		if (target.empty() ||
			target[0] != '/' ||
			target.find("..") != beast::string_view::npos)
			return send(bad_request("Illegal request-target"));

		// Build the path to the requested file
		std::string path = path_cat(doc_root, target);
		if (target.back() == '/')
			path.append("login.html");


		// Attempt to open the file
		body.open(path.c_str(), beast::file_mode::scan, ec);


		// Handle the case where the file doesn't exist
		if (ec == beast::errc::no_such_file_or_directory)
			return send(not_found(target));

		// Handle an unknown error
		if (ec)
			return send(server_error(ec.message()));

		// Cache the size since we need it after the move
		auto const size = body.size();

		// Respond to HEAD request
		if (method == http::verb::head)
		{
			http::response<http::empty_body> res{ http::status::ok, req.version() };
			res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
			res.set(http::field::content_type, mime_type(path));
			res.content_length(size);
			res.keep_alive(req.keep_alive());


			
			return send(std::move(res));
		}

		// Respond to GET request
		http::response<http::file_body> res{
			std::piecewise_construct,
			std::make_tuple(std::move(body)),
			std::make_tuple(http::status::ok, req.version()) };

		res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		res.set(http::field::content_type, mime_type(path));
		res.content_length(size);
		res.keep_alive(req.keep_alive());
		
		return send(std::move(res));
	}
}


//------------------------------------------------------------------------------

// Report a failure
void 
fail(beast::error_code ec, char const* what)
{
	std::cerr << what << ": " << ec.message() << "\n";
}




// Echoes back all received WebSocket messages
class websocket_session : public std::enable_shared_from_this<websocket_session>
{
	websocket::stream<beast::tcp_stream> ws_;
	beast::flat_buffer buffer_;

public:
	// Take ownership of the socket
	explicit 
		websocket_session(tcp::socket&& socket)
		: ws_(std::move(socket))
	{
	}



	// Start the asynchronous accept operation
	template<class Body, class Allocator>
	void 
		do_accept(http::request<Body, http::basic_fields<Allocator>> req)
	{

		// Set suggested timeout settings for the websocket
		ws_.set_option(websocket::stream_base::timeout::suggested(beast::role_type::server));

		// Set a decorator to change the Server of the handshake
		ws_.set_option(websocket::stream_base::decorator(
			[](websocket::response_type& res)
		{
			res.set(http::field::server,std::string(BOOST_BEAST_VERSION_STRING) +" advanced-server");
		}));

		// Accept the websocket handshake
		ws_.async_accept(req,beast::bind_front_handler(&websocket_session::on_accept,shared_from_this()));
	}

private:


	void 
		on_accept(beast::error_code ec)
	{
		if (ec)
			return fail(ec, "accept");

		// Read a message
		do_read();
	}



	void 
		do_read()
	{
		
		// Read a message into our buffer
		ws_.async_read(buffer_,beast::bind_front_handler(&websocket_session::on_read,shared_from_this()));
	}


	void 
		on_read(beast::error_code ec,std::size_t bytes_transferred)
	{
		boost::ignore_unused(bytes_transferred);

		// This indicates that the websocket_session was closed
		if (ec == websocket::error::closed)
			return;

		if (ec)
			fail(ec, "read");

		// Echo the message
		ws_.text(ws_.got_text());
		ws_.async_write(buffer_.data(),beast::bind_front_handler(&websocket_session::on_write,shared_from_this()));

	}



	void 
		on_write(beast::error_code ec,std::size_t bytes_transferred)
	{
	
		boost::ignore_unused(bytes_transferred);

		if (ec)
			return fail(ec, "write");

		// Clear the buffer
		buffer_.consume(buffer_.size());

		// Do another read
		do_read();
	}
};




//------------------------------------------------------------------------------

// Handles an HTTP server connection
class http_session : public std::enable_shared_from_this<http_session>
{
	// This queue is used for HTTP pipelining.
	class queue
	{
		enum
		{
			// Maximum number of responses we will queue
			limit = 8
		};

		// The type-erased, saved work item
		struct work
		{
			virtual ~work() = default;
			virtual void operator()() = 0;
		};

		http_session& self_;
		std::vector<std::unique_ptr<work>> items_;

	public:
		explicit 
			queue(http_session& self) : self_(self)
		{
			

			static_assert(limit > 0, "queue limit must be positive");
			items_.reserve(limit);
		}

		// Returns `true` if we have reached the queue limit
		bool 
			is_full() const
		{
			
			return items_.size() >= limit;
		}

		// Called when a message finishes sending
		// Returns `true` if the caller should initiate a read
		bool 
			on_write()
		{
			

			BOOST_ASSERT(!items_.empty());
			auto const was_full = is_full();
			items_.erase(items_.begin());
			if (!items_.empty())
				(*items_.front())();
			return was_full;
		}

		// Called by the HTTP handler to send a response.
		template<bool isRequest, class Body, class Fields>
		void 
			operator()(http::message<isRequest, Body, Fields>&& msg)
		{
			// This holds a work item
			struct work_impl : work
			{
				http_session& self_;
				http::message<isRequest, Body, Fields> msg_;

				work_impl(http_session& self,http::message<isRequest, Body, Fields>&& msg): self_(self), msg_(std::move(msg))
				{

				}


				void operator()()
				{
					http::async_write(self_.stream_,msg_,beast::bind_front_handler(&http_session::on_write,self_.shared_from_this(),msg_.need_eof()));
				}
			};

			// Allocate and store the work
			items_.push_back(boost::make_unique<work_impl>(self_, std::move(msg)));

			// If there was no previous work, start this one
			if (items_.size() == 1)
				(*items_.front())();
		}
	};




	beast::tcp_stream stream_;
	beast::flat_buffer buffer_;
	std::shared_ptr<std::string const> doc_root_;
	queue queue_;

	// The parser is stored in an optional container so we can
	// construct it from scratch it at the beginning of each new message.
	boost::optional<http::request_parser<http::string_body>> parser_;
	shared_ptr<SQL>m_sql;
	shared_ptr<io_context::strand>m_strand;


public:
	// Take ownership of the socket
	http_session(tcp::socket&& socket,std::shared_ptr<std::string const> const& doc_root , shared_ptr<SQL> sql ,
		shared_ptr<io_context::strand> strand ): stream_(std::move(socket)), doc_root_(doc_root), queue_(*this)
	{
		m_sql = sql;

		m_strand = strand;
	}

	// Start the session
	void
		run()
	{
		do_read();
	}

private:
	void 
		do_read()
	{

		// Construct a new parser for each message
		parser_.emplace();

		// Apply a reasonable limit to the allowed size
		// of the body in bytes to prevent abuse.
		parser_->body_limit(1024);

		// Set the timeout.
		stream_.expires_after(std::chrono::seconds(30));

		// Read a request using the parser-oriented interface
		http::async_read(stream_,buffer_,*parser_,beast::bind_front_handler(&http_session::on_read,shared_from_this()));
	}


	void 
		on_read(beast::error_code ec, std::size_t bytes_transferred)
	{
		
		boost::ignore_unused(bytes_transferred);

		// This means they closed the connection
		if (ec == http::error::end_of_stream)
			return do_close();

		if (ec)
			return fail(ec, "read");

		int size = buffer_.size();

		buffer_.consume(size);

	
			// See if it is a WebSocket Upgrade
		if (websocket::is_upgrade(parser_->get()))
		{
			// Create a websocket session, transferring ownership

			// of both the socket and the HTTP request.
			std::make_shared<websocket_session>(stream_.release_socket())->do_accept(parser_->release());
			return;
		}

		// Send the response
		handle_request(*doc_root_, std::move(parser_->release()), queue_ ,m_sql, m_strand );

		// If we aren't at the queue limit, try to pipeline another request
		if (!queue_.is_full())
			do_read();
	}


	void 
		on_write(bool close, beast::error_code ec, std::size_t bytes_transferred)
	{
		
		boost::ignore_unused(bytes_transferred);

		if (ec)
			return fail(ec, "write");

		if (close)
		{
			// This means we should close the connection, usually because
			// the response indicated the "Connection: close" semantic.
			return do_close();
		}

		// Inform the queue that a write completed
		if (queue_.on_write())
		{
			// Read another request
			do_read();
		}
	}


	void 
		do_close()
	{	
		// Send a TCP shutdown
		beast::error_code ec;
		stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
		// At this point the connection is closed gracefully
	}
};












//------------------------------------------------------------------------------

// Accepts incoming connections and launches the sessions
class listener : public std::enable_shared_from_this<listener>
{
	net::io_context& ioc_;
	tcp::acceptor acceptor_;
	std::shared_ptr<std::string const> doc_root_;
	shared_ptr<SQL> m_sql;
	shared_ptr<io_context::strand> m_strand;


public:
	listener(net::io_context& ioc,tcp::endpoint endpoint,std::shared_ptr<std::string const> const& doc_root , shared_ptr<SQL>sql ,
	shared_ptr<io_context::strand> strand ): ioc_(ioc), acceptor_(net::make_strand(ioc)), doc_root_(doc_root)
	{	
		beast::error_code ec;

		m_sql = sql;

		m_strand = strand;

		// Open the acceptor
		acceptor_.open(endpoint.protocol(), ec);
		if (ec)
		{
			fail(ec, "open");
			return;
		}

		// Allow address reuse
		acceptor_.set_option(net::socket_base::reuse_address(true), ec);
		if (ec)
		{
			fail(ec, "set_option");
			return;
		}

		// Bind to the server address
		acceptor_.bind(endpoint, ec);
		if (ec)
		{
			fail(ec, "bind");
			return;
		}

		// Start listening for connections
		acceptor_.listen(net::socket_base::max_listen_connections, ec);
		if (ec)
		{
			fail(ec, "listen");
			return;
		}
	}

	// Start accepting incoming connections
	void 
		run()
	{	
		do_accept();
	}


private:
	void 
		do_accept()
	{	
		// The new connection gets its own strand
		acceptor_.async_accept(net::make_strand(ioc_),beast::bind_front_handler(&listener::on_accept,shared_from_this()));
	}

	void 
		on_accept(beast::error_code ec, tcp::socket socket)
	{	
		if (ec)
		{	
			fail(ec, "accept");
		}
		else
		{
			// Create the http session and run it
			std::make_shared<http_session>(
				std::move(socket),
				doc_root_ , m_sql , m_strand )->run();
		}

		// Accept another connection
		do_accept();
	}
};






//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	
	boost::asio::ip::address address;
	unsigned short port;
	std::shared_ptr<std::string>doc_root;
	int threads;

	// Check command line arguments.
	if (argc == 5)
	{
		address = net::ip::make_address(argv[1]);
		port = static_cast<unsigned short>(std::atoi(argv[2]));
		doc_root = std::make_shared<std::string>(argv[3]);
		threads = std::max<int>(1, std::atoi(argv[4]));
	}
	else
	{
		std::string addressStr, portStr, rootDirStr, threadNum;

#ifdef BOOST_OS_LINUX
		addressStr = "0.0.0.0", portStr = "8080", rootDirStr = "/home/testHTTP", threadNum = "8";
#elif defined BOOST_OS_WINDOWS
		addressStr = "127.0.0.1", portStr = "8080", rootDirStr = "D:/testHTTP", threadNum = "8";
#endif

		address = net::ip::make_address(addressStr);
		port = static_cast<unsigned short>(std::stoi(portStr));
		doc_root = std::make_shared<std::string>(rootDirStr);
		threads = std::max<int>(1, std::stoi(threadNum));
	}


	// The io_context is required for all I/O
	net::io_context ioc{ threads };


	shared_ptr<SQL>m_sql;
	shared_ptr<io_context::strand>m_strand;

	m_sql.reset(new SQL());
	m_strand.reset(new io_context::strand(ioc));



	if (m_sql->ConnectDatabase())
	{
		std::cout << "connect mysql success\n";
	}



	// Create and launch a listening port
	std::make_shared<listener>(
		ioc,
		tcp::endpoint{ address, port },
		doc_root , m_sql, m_strand)->run();



#ifdef BOOST_OS_LINUX
	// Capture SIGINT and SIGTERM to perform a clean shutdown
	net::signal_set signals(ioc, SIGINT, SIGTERM);
	signals.async_wait(
		[&](beast::error_code const&, int)
	{
		// Stop the `io_context`. This will cause `run()`
		// to return immediately, eventually destroying the
		// `io_context` and all of the sockets in it.
		ioc.stop();
	}   );
#endif

	// Run the I/O service on the requested number of threads
	std::vector<std::thread> v;
	v.reserve(threads - 1);
	for (auto i = threads - 1; i > 0; --i)
		v.emplace_back(
			[&ioc]
	{
		ioc.run();
	});

	// (If we get here, it means we got a SIGINT or SIGTERM)

	// Block until all the threads exit
	for (auto& t : v)
		t.join();

	return 0;
}




