
#pragma once

#include <time.h>
#include <ctime>
#include <json.hpp>

#include "Point.hpp"

namespace RouteStat {

	class RoutePoint {

	private:

		Point		_point;
		int			_dist;
		std::time_t	_time;
		int			_duration;


	public:

		RoutePoint();
		RoutePoint(
			double la, double lo, int dist, std::time_t time, int duration);
		RoutePoint(nlohmann::json json);

		// getters/setters

		double		getLat() const;
		double		getLong() const;
		Point &		getPoint();
		time_t		getTime() const;

		void		setLat(double _lat);
		void		setLong(double _long);
		void		setTime(time_t _time);
	};
}
