
#include "RoutePoint.hpp"

RoutePoint::RoutePoint() = default;

RoutePoint::RoutePoint(double _lat, double _long, std::time_t _time)
: _point(_lat, _long), _time(_time) { }

double	RoutePoint::getLat() const { return (_point.getLat()); }
double	RoutePoint::getLong() const { return (_point.getLong()); }
time_t	RoutePoint::getTime() const { return _time; }

void	RoutePoint::setLat(double _lat) { _point.setLat(_lat); }
void	RoutePoint::setLong(double _long) { _point.setLong(_long); }
void	RoutePoint::setTime(time_t _time) { RoutePoint::_time = _time; }
