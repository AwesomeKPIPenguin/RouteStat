
#pragma once

#include <vector>
#include <json.hpp>
#include "Point.hpp"

using json = nlohmann::json;

namespace routeStat {

	class Polygon {

	private:

		std::vector<Point>	_points;
		double				_minLat;
		double				_maxLat;
		double				_minLong;
		double				_maxLong;

	public:

		explicit Polygon(json &arr);

		// getters

		std::vector<Point>	getPoints() const;
		double				getMinLat() const;
		double				getMaxLat() const;
		double				getMinLong() const;
		double				getMaxLong() const;
	};
}
