
#pragma once

#include <ctime>
#include "Point.hpp"

namespace RouteStat {

	class RoutePoint {

	private:

		std::time_t	_time;
		Point		_point;

		RoutePoint();

	public:

		RoutePoint(double _lat, double _long, std::time_t _time);

		// getters/setters

		double		getLat() const;
		double		getLong() const;
		time_t		getTime() const;

		void		setLat(double _lat);
		void		setLong(double _long);
		void		setTime(time_t _time);
	};
}
