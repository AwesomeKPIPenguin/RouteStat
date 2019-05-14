
#pragma once

#include <string>
#include <zconf.h>
#include <zmq.hpp>

#include "DB.hpp"
#include "Polygon.hpp"
#include "RoutePoint.hpp"

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
		json			_res;

		void			checkConnections();
		void			handlePolygon(
							DB &db, std::vector<Polygon> *map, json json);
		void			handleRoute(
							std::vector<Polygon> *map, json json);

	public:

		Connection(std::string, std::string, std::string);

		void			listen(DB &db, std::vector<Polygon> *map);
		void			addPoint(
							RoutePoint p1, RoutePoint p2,
							Point &inter, int id, std::string position);
		void			send();
	};
}
