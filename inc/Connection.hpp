
#pragma once

#include <iostream>
#include <string>
#include <zconf.h>
#include <zmq.hpp>

namespace RouteStat {

	class Connection {

	private:

		static const std::string	PROTOCOL;

		std::string					_host;
		std::string					_sub;
		std::string					_pub;
		zmq::context_t				_context;
		zmq::socket_t				_subs;
		zmq::socket_t				_pubs;

	public:

		Connection(std::string, std::string, std::string);
	};
}
