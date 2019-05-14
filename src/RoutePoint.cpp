
#include "RoutePoint.hpp"

namespace RouteStat {

	RoutePoint::RoutePoint() = default;

	RoutePoint::RoutePoint(
		double la, double lo, int dist, std::time_t time, int duration)
	: _point(la, lo), _dist(dist), _time(time), _duration(duration) { }

	RoutePoint::RoutePoint(nlohmann::json json)
	: _point(json[0].get<double>(), json[1].get<double>()),
	  _dist(json[2].get<int>()),
	  _duration(json[4].get<int>()) {

		struct tm tm;
		strptime(json[3].get<std::string>().c_str(), "%H:%M:%S", &tm);
		_time = mktime(&tm);
	}

	double	RoutePoint::getLat() const { return (_point.getLa()); }
	double	RoutePoint::getLong() const { return (_point.getLo()); }
	Point &	RoutePoint::getPoint() { return (_point); }
	time_t	RoutePoint::getTime() const { return _time; }

	void	RoutePoint::setLat(double _lat) { _point.setLa(_lat); }
	void	RoutePoint::setLong(double _long) { _point.setLo(_long); }
	void	RoutePoint::setTime(time_t _time) { RoutePoint::_time = _time; }
}
