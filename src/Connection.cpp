
#include "Connection.hpp"

namespace RouteStat {

	const std::string	Connection::PROTOCOL = "tcp";

	void	Connection::checkConnections() {

		if (_subs.connected())
			std::cout << "Successfully subscribed to         ["
					  << PROTOCOL << "://" << _host << ":" << _sub << "]"
					  << std::endl;
		else
			std::cout << "CONNECTION ERROR: unable to subscribe to ["
					  << PROTOCOL << "://" << _host << ":" << _sub << "]"
					  << std::endl;
		if (_pubs.connected())
			std::cout << "Successfully binded publisher to   ["
					  << PROTOCOL << "://" << _host << ":" << _pub << "]"
					  << std::endl;
		else
			std::cout << "CONNECTION ERROR: unable to bind publisher to ["
					  << PROTOCOL << "://" << _host << ":" << _pub << "]"
					  << std::endl;
	}

	Connection::Connection(std::string host, std::string pub, std::string sub)
		: _host(std::move(host)),
		  _pub(std::move(pub)),
		  _sub(std::move(sub)),
		  _context(zmq::context_t(1)),
		  _pubs(zmq::socket_t(_context, ZMQ_PUB)),
		  _subs(zmq::socket_t(_context, ZMQ_SUB)) {

		_pubs.bind(PROTOCOL + "://" + _host + ":" + _pub);
		_subs.connect(PROTOCOL + "://" + _host + ":" + _sub);
		_subs.setsockopt(ZMQ_SUBSCRIBE, "", 0);
		usleep(100);
		checkConnections();
	}

	void	Connection::listen(DB &db, std::vector<Polygon> *map) {

		zmq::message_t	msg;
		nlohmann::json	json;

		while (true) {

			_subs.recv(&msg);
			json = nlohmann::json((char *)msg.data());
			if (json[0].is_array())
				handleRoute(json);
			else
				handlePolygon(db, map, json);
		}
	}

	void	Connection::handlePolygon(
				DB &db, std::vector<Polygon> *map, nlohmann::json json) {

		Polygon	poly;

		poly = Polygon(
			std::stoi(json[0]["name"].get<std::string>()),
			json[0]["coords"][0]);

		for (Polygon p : *map) {

			if (!poly.isNear(p))
				continue ;

			for (Point point : *(p.getPoints())) {

				if (point.isInPoly(poly)) {

					std::cerr << "ERROR HANDLING POLYGON: received polygon has "
							  << "intersections with present map" << std::endl;
					return ;
				}
			}
			for (Point point : *(poly.getPoints())) {

				if (point.isInPoly(p)) {

					std::cerr << "ERROR HANDLING POLYGON: received polygon has "
							  << "intersections with present map" << std::endl;
					return ;
				}
			}
		}

		map->emplace_back(poly);
		db.insert(poly.getId(), json[0]["coords"][0]);
	}
}
