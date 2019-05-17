
#include "RoutePoint.hpp"

namespace RouteStat {

	RoutePoint::RoutePoint() = default;

	RoutePoint::RoutePoint(
		double la, double lo, int dist, tm &time, int duration)
	: _point(la, lo), _dist(dist), _time(time), _duration(duration) { }

	RoutePoint::RoutePoint(nlohmann::json json)
	: _point(json[0].get<double>(), json[1].get<double>()),
	  _dist(json[2].get<int>()),
	  _duration(json[4].get<int>()) {

		strptime(json[3].get<std::string>().c_str(), "%H:%M:%S", &_time);
		std::cout << "Parsed time: " << asctime(&_time) << std::endl;
	}

	double			RoutePoint::getLat() const { return (_point.getLa()); }
	double			RoutePoint::getLong() const { return (_point.getLo()); }
	Point &			RoutePoint::getPoint() { return (_point); }
	int				RoutePoint::getDist() const { return (_dist); }
	tm *			RoutePoint::getTime() { return &_time; }
	int				RoutePoint::getDuration() const	{ return (_duration); }

	void			RoutePoint::setLat(double la) { _point.setLa(la); }
	void			RoutePoint::setLong(double lo) { _point.setLo(lo); }
	void			RoutePoint::setTime(tm &time) { _time = time; }

	std::ostream &	operator<<(std::ostream &os, const RoutePoint &p) {

		char	timestr[9];

		timestr[8] = 0;
		strftime(timestr, 8, "%H:%M:%S", &p._time);
		return (os << "[" << p._point.getLa() << ", "
						  << p._point.getLo() << ", "
						  << p._dist << ", "
						  << "\"" << timestr << "\"" << ", "
						  << p._duration
				   << "]");
	}



	Polygon *		RoutePoint::handleRouteEndPoint(
						Connection *conn,
						std::vector<Polygon> *map,
						RoutePoint &rp) {

		Polygon	*res;

		res = nullptr;
		for (Polygon &poly : *map) {

			if (rp.getPoint().isInPoly(poly)) {

				res = &poly;
				break ;
			}
		}
		conn->addPoint(
			rp, rp, rp.getPoint(), (res) ? res->getId() : -1, " ");
		return (res);
	}
}
