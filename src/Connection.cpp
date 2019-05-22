
#include "Connection.hpp"

namespace RouteStat {

	const std::string	Connection::PROTOCOL = "tcp";

	void		Connection::checkConnections() {

		if (_subs.connected())
			std::cout << "Successfully subscribed to             ["
					  << PROTOCOL << "://" << _host << ":" << _sub << "]"
					  << std::endl;
		else
			std::cout << "CONNECTION ERROR: unable to subscribe to ["
					  << PROTOCOL << "://" << _host << ":" << _sub << "]"
					  << std::endl;
		if (_pubs.connected())
			std::cout << "Successfully binded publisher to       ["
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
		  _subs(zmq::socket_t(_context, ZMQ_SUB)),
		  _res("[]"_json) {

		_pubs.bind(PROTOCOL + "://" + _host + ":" + _pub);
		_subs.connect(PROTOCOL + "://" + _host + ":" + _sub);
		_subs.setsockopt(ZMQ_SUBSCRIBE, "", 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		checkConnections();
	}

	void		Connection::addPoint(
					RoutePoint p1, RoutePoint p2,
					Point &inter, int id, std::string position) {

		double	dist;
		double	len;
		char	timeStr[10];
		time_t	t[3];

		bzero(timeStr, 10);
		if (position == " ") {

			strftime(timeStr, 10, "%H:%M:%S", p1.getTime());
			_res.emplace_back(json::array({
				inter.getLa(),
				inter.getLo(),
				p1.getDist(),
				timeStr,
				p1.getDuration(),
				(id >= 0) ? std::to_string(id) : "",
				position
			}));
			return ;
		}
		len = p1.getPoint().getDist(p2.getPoint());
		dist = p1.getPoint().getDist(inter);
		t[0] = mktime(p1.getTime());
		t[1] = mktime(p2.getTime());
		t[2] = (time_t)(t[0] + difftime(t[1], t[0]) * dist / len);
		strftime(timeStr, 10, "%H:%M:%S", localtime(&t[2]));
		_res.emplace_back(json::array({
			inter.getLa(),
			inter.getLo(),
			(int)(dist * 10e5),
			timeStr,
			(int)(p1.getDuration() + difftime(t[2], t[0])),
			(id >= 0) ? std::to_string(id) : "",
			position
		}));
	}

	void		Connection::send() {

		zmq::message_t	msg;
		std::string		str;

		str = _res.dump();
		msg = zmq::message_t(str.c_str(), str.length());
		_pubs.send(msg);
	}

	void		Connection::listenForExit() {

		std::string		input;

		while (true) {

			std::cin >> input;
			if (input == "exit")
				exit(EXIT_SUCCESS);
		}
	}

	void		Connection::listen(DB &db, std::vector<Polygon> *map) {

		std::thread		thread;
		zmq::message_t	msg;
		json			json;

		thread = std::thread([&]() { listenForExit(); });
		while (true) {

			_subs.recv(&msg);
			json = nlohmann::json::parse((char *)msg.data());
			if (json[0].is_array())
				handleRoute(map, json);
			else
				handlePolygon(db, map, json);
		}
	}

	void		Connection::handlePolygon(
					DB &db, std::vector<Polygon> *map, json json) {

		Polygon	poly;

		poly = Polygon(
			std::stoi(json[0]["name"].get<std::string>()),
			json[0]["coords"][0]);

		for (Polygon p : *map)
			if (poly.isInterPoly(p))
				return ;

		map->emplace_back(poly);
		db.insert(poly.getId(), json[0]["coords"][0]);
		std::cout << "Polygon '" << poly.getId() << "' successfully added"
				  << std::endl;
	}

	Polygon *	Connection::handleRouteEndPoint(
					std::vector<Polygon> *map, RoutePoint &rp) {

		Polygon	*res;

		res = nullptr;
		for (Polygon &poly : *map) {

			if (rp.getPoint().isInPoly(poly)) {

				res = &poly;
				break ;
			}
		}
		addPoint(
			rp, rp, rp.getPoint(), (res) ? res->getId() : -1, " ");
		return (res);
	}

	void		Connection::handleRouteInside(
					RoutePoint &p1, RoutePoint &p2,
					Polygon **cp, std::vector<Polygon> *map) {

		bool	foundNeighbor;
		Point	res;

		foundNeighbor = false;
		if (Point::segmentPolyIntersection(
			p1.getPoint(), p2.getPoint(), **cp, res)) {

			addPoint(p1, p2, res, (*cp)->getId(), "OUT");
			for (Polygon &poly : *map) {

				if (&poly == (*cp))
					continue ;
				if (p2.getPoint().isInPoly(poly)) {

					(*cp) = &poly;
					foundNeighbor = true;
					addPoint(p1, p2, res, (*cp)->getId(), "IN");
					break ;
				}
			}
			if (!foundNeighbor)
				*cp = nullptr;
		}
	}

	void		Connection::handleRouteOutside(
					RoutePoint &p1, RoutePoint &p2,
					Polygon **cp, std::vector<Polygon> *map) {

		Point		res;

		for (Polygon &poly : *map) {

			if (!p1.getPoint().isNearPoly(poly)
				&& !p2.getPoint().isNearPoly(poly))
				continue ;

			if (Point::segmentPolyIntersection(
				p1.getPoint(), p2.getPoint(), poly, res)) {

				*cp = &poly;
				addPoint(p1, p2, res, (*cp)->getId(), "IN");
				break ;
			}
		}
	}

	void		Connection::handleRoute(std::vector<Polygon> *map, json json) {

		Polygon		*currPoly;
		RoutePoint	segment[2];

		std::cout << "Accepted new route" << std::endl;

		currPoly = nullptr;
		segment[0] = RoutePoint(json[0]);
		segment[1] = segment[0];

		currPoly = handleRouteEndPoint(map, segment[0]);
		for (int i = 1; i < json.size(); ++i) {

			segment[0] = segment[1];
			segment[1] = RoutePoint(json[i]);
			if (currPoly)
				handleRouteInside(segment[0], segment[1], &currPoly, map);
			else
				handleRouteOutside(segment[0], segment[1], &currPoly, map);
		}
		handleRouteEndPoint(map, segment[1]);
		send();
	}
}
