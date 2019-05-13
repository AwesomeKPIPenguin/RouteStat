
#pragma once

#include <string>
#include <zconf.h>
#include <zmq.hpp>

#include "DB.hpp"
#include "Polygon.hpp"

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
		void			handlePolygon(DB &, std::vector<Polygon> *, nlohmann::json);
		void			handleRoute(nlohmann::json);

	public:

		Connection(std::string, std::string, std::string);

		void			listen(DB &, std::vector<Polygon> *);
	};
}
