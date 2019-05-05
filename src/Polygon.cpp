
#include "Polygon.hpp"

namespace routeStat {

	Polygon::Polygon(json &j)
		: _minLat(std::numeric_limits<double>::max()),
		  _maxLat(-std::numeric_limits<double>::max()),
		  _minLong(std::numeric_limits<double>::max()),
		  _maxLong(-std::numeric_limits<double>::max()) {

		json	arr;
		Point	p;
		Point	*prev1;
		Point	*prev2;

		arr = j[0]["coords"][0];
		prev1 = nullptr;
		prev2 = nullptr;
		for (const auto &coords : arr) {

			p.setLat(coords[0].get<double>());
			p.setLong(coords[1].get<double>());

			if (p.getLat() < _minLat)
				_minLat = p.getLat();
			else if (p.getLat() > _maxLat)
				_maxLat = p.getLat();
			if (p.getLong() < _minLong)
				_minLong = p.getLong();
			else if (p.getLong() > _maxLong)
				_maxLong = p.getLong();

			// checks if the third point is on one line with two previous -
			//   middle one is removed then due to it's redundancy

			if (prev2 && p.isInLine(*prev2, *prev1)) {

				std::cout << "DROPPED" << std::endl;

				_points.pop_back();
				prev1 = &_points.back();
			}

			_points.emplace_back(p);

			if (prev1)
				prev2 = &_points.end()[-2];
			prev1 = &_points.end()[-1];
		}
	}

	std::vector<Point>	Polygon::getPoints() const { return (_points); }
	double				Polygon::getMinLat() const { return (_minLat); }
	double				Polygon::getMaxLat() const { return (_maxLat); }
	double				Polygon::getMinLong() const { return (_minLong); }
	double				Polygon::getMaxLong() const { return (_maxLong); }
}
