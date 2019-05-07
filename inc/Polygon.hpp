
#pragma once

#include <vector>
#include <json.hpp>

#include "Point.hpp"

using json = nlohmann::json;

namespace routeStat {

	class Polygon {

	private:

		int					_id;
		std::vector<Point>	_points;
		double				_minLa;
		double				_maxLa;
		double				_minLo;
		double				_maxLo;

	public:

		explicit Polygon(json &);	// "[[la, lo], [la, lo], ...]"

		// getters

		int					getId() const;
		std::vector<Point>	getPoints() const;
		double				getMinLa() const;
		double				getMaxLa() const;
		double				getMinLo() const;
		double				getMaxLo() const;
	};
}
