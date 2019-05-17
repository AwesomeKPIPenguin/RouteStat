
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
		tm			_time;
		int			_duration;


	public:

		RoutePoint();
		RoutePoint(
			double la, double lo, int dist, tm &time, int duration);
		explicit RoutePoint(nlohmann::json json);

		// operators

		friend std::ostream &	operator<<(std::ostream &, const RoutePoint &);

		// getters/setters

		double					getLat() const;
		double					getLong() const;
		Point &					getPoint();
		int						getDist() const;
		tm *					getTime();
		int						getDuration() const;

		void					setLat(double la);
		void					setLong(double lo);
		void					setTime(tm &time);
	};
}
