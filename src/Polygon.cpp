
#include "Polygon.hpp"

namespace routeStat {

	Polygon::Polygon(json & j)
		: _minLa(std::numeric_limits<double>::max()),
		  _maxLa(-std::numeric_limits<double>::max()),
		  _minLo(std::numeric_limits<double>::max()),
		  _maxLo(-std::numeric_limits<double>::max()) {

		Point	p;
		Point	*prev1;
		Point	*prev2;

		prev1 = nullptr;
		prev2 = nullptr;
		for (const auto & coords : j) {

			p.setLa(coords[0].get<double>());
			p.setLo(coords[1].get<double>());

			if (p.getLa() < _minLa)
				_minLa = p.getLa();
			else if (p.getLa() > _maxLa)
				_maxLa = p.getLa();
			if (p.getLo() < _minLo)
				_minLo = p.getLo();
			else if (p.getLo() > _maxLo)
				_maxLo = p.getLo();

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

	int					Polygon::getId() const { return (_id); }
	std::vector<Point>	Polygon::getPoints() const { return (_points); }
	double				Polygon::getMinLa() const { return (_minLa); }
	double				Polygon::getMaxLa() const { return (_maxLa); }
	double				Polygon::getMinLo() const { return (_minLo); }
	double				Polygon::getMaxLo() const { return (_maxLo); }
}
