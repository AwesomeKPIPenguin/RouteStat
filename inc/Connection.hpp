
#pragma once

#include <string>
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

		void			checkConnections();
		void			listenForExit();
		void			handlePolygon(
							DB &db, std::vector<Polygon> *map, json json);
		void			handleRoute(
							std::vector<Polygon> *map, json json);
		Polygon *		handleRouteEndPoint(
							std::vector<Polygon> *map, RoutePoint &rp);
		void			handleRouteInside(
							RoutePoint &p1, RoutePoint &p2,
							Polygon **cp, std::vector<Polygon> *map);
		void			handleRouteOutside(
							RoutePoint &p1, RoutePoint &p2,
							Polygon **cp, std::vector<Polygon> *map);

	public:

		Connection(std::string host, std::string pub, std::string sub);

		void			listen(DB &db, std::vector<Polygon> *map);
		void			addPoint(
							RoutePoint p1, RoutePoint p2,
							Point &inter, int id, std::string position);
		void			send();
	};
}
