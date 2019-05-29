
#pragma once

#include <string>
#include <zmq.hpp>

#include "DB.hpp"
#include "Polygon.hpp"
#include "RoutePoint.hpp"
#include "Handlers.hpp"

namespace RouteStat {

	class Connection {

	private:

		static const std::string	PROTOCOL;

		std::string		_host;
		std::string		_sub;
		std::string		_pub;
		zmq::context_t	_context;
		zmq::socket_t	_subs;
		zmq::socket_t	_pubs;

		void			checkConnections();
		void			listenForExit();

	public:

		Connection(std::string host, std::string pub, std::string sub);

		void			listen(DB &db, std::vector<Polygon> *map);
		void			send(json &respond);
	};
}
