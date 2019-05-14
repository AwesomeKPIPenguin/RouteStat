
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
		  _subs(zmq::socket_t(_context, ZMQ_SUB)),
		  _res("[]"_json) {

		_pubs.bind(PROTOCOL + "://" + _host + ":" + _pub);
		_subs.connect(PROTOCOL + "://" + _host + ":" + _sub);
		_subs.setsockopt(ZMQ_SUBSCRIBE, "", 0);
		usleep(100);
		checkConnections();
	}

	void	Connection::addPoint(
				RoutePoint p1, RoutePoint p2,
				Point &inter, int id, std::string position) {

		double	dist;
		double	len;
		char	timeStr[8];
		time_t	time;
		tm		tm;

		len = p1.getPoint().getDist(p2.getPoint());
		dist = p1.getPoint().getDist(inter);
		time = (size_t)(p1.getTime() + (double)p1.getTime() * dist / len);
		tm = *localtime(&time);
		strftime(timeStr, 8, "%H:%M:%S", &tm);
		_res.emplace_back(json::array({
			inter.getLa(),
			inter.getLo(),
			(int)(dist * 10e5),
			timeStr,
			time - p1.getTime(),
			(id >= 0) ? std::to_string(id) : "",
			position
		}));
	}

	void	Connection::send() {

		zmq::message_t	msg;
		std::string		str;

		str = _res.dump();
		msg = zmq::message_t(str.c_str(), str.length());
		_pubs.send(msg);
	}

	void	Connection::listen(DB &db, std::vector<Polygon> *map) {

		zmq::message_t	msg;
		json			json;

		while (true) {

			_subs.recv(&msg);
			json = nlohmann::json((char *)msg.data());
			if (json[0].is_array())
				handleRoute(map, json);
			else
				handlePolygon(db, map, json);
		}
	}

	void	Connection::handlePolygon(
				DB &db, std::vector<Polygon> *map, json json) {

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

	void	Connection::handleRoute(std::vector<Polygon> *map, json json) {

		Polygon		*currPoly;
		RoutePoint	segment[2];
		Point		res;

		currPoly = nullptr;
		segment[0] = RoutePoint(json[0]);
		for (Polygon &poly : *map) {

			if (segment[0].getPoint().isInPoly(poly)) {

				currPoly = &poly;
				break ;
			}
		}
		for (int i = 1; i < json.size(); ++i) {

			segment[0] = segment[1];
			segment[1] = RoutePoint(json[i]);

			if (currPoly) {

				if (!segment[0].getPoint().isNearPoly(*currPoly)
					&& !segment[1].getPoint().isNearPoly(*currPoly))
					continue ;

				if (Point::segmentPolyIntersection(
					segment[0].getPoint(), segment[1].getPoint(), *currPoly, res)) {

					addPoint(segment[0], segment[1], res, currPoly->getId(), "OUT");
					currPoly = nullptr;
					for (Polygon &poly : *map) {

						if (&poly == currPoly)
							continue ;

						if (segment[1].getPoint().isInPoly(poly)) {

							currPoly = &poly;
							addPoint(segment[0], segment[1], res, currPoly->getId(), "IN");
							break ;
						}
					}
				}

			}
			else {

				for (Polygon &poly : *map) {

					if (!segment[0].getPoint().isNearPoly(poly)
						&& !segment[1].getPoint().isNearPoly(poly))
						continue ;

					if (Point::segmentPolyIntersection(
						segment[0].getPoint(), segment[1].getPoint(), poly, res)) {

						currPoly = &poly;
						addPoint(segment[0], segment[1], res, currPoly->getId(), "IN");
						break ;
					}
				}
			}
		}
		send();
	}
}
