
#include "Polygon.hpp"

namespace RouteStat {

	Polygon::Polygon() = default;

	Polygon::Polygon(int id, json j)
		: _id(id),
		  _points(new std::vector<Point>()),
		  _minLa(std::numeric_limits<double>::max()),
		  _maxLa(-std::numeric_limits<double>::max()),
		  _minLo(std::numeric_limits<double>::max()),
		  _maxLo(-std::numeric_limits<double>::max()) {

		Point	p;
		Point	*prev1;
		Point	*prev2;

		_points->reserve(10000);
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
			// middle one is removed then due to it's redundancy

			if (prev2 && p.isInLine(*prev2, *prev1)) {

				_points->pop_back();
				prev1 = &_points->back();
			}

			_points->emplace_back(p);

			if (prev1)
				prev2 = &_points->end()[-2];
			prev1 = &_points->end()[-1];
		}
	}

	int						Polygon::getId() const { return (_id); }
	std::vector<Point> *	Polygon::getPoints() const { return (_points); }
	double					Polygon::getMinLa() const { return (_minLa); }
	double					Polygon::getMaxLa() const { return (_maxLa); }
	double					Polygon::getMinLo() const { return (_minLo); }
	double					Polygon::getMaxLo() const { return (_maxLo); }



	bool					Polygon::isNear(const Polygon &p) const {

		return (
			((_minLa <= p._maxLa && _minLa >= p._minLa)
				|| (_maxLa <= p._maxLa && _maxLa >= p._minLa))
			&& ((_minLo <= p._maxLo && _minLo >= p._minLo)
				|| (_maxLo <= p._maxLo && _maxLo >= p._minLo)));
	}

	bool					Polygon::isInterPoly(const Polygon &poly) const {

		std::vector<Point>	*points;

		if (_id == poly._id) {

			std::cerr << "ERROR HANDLING POLYGON: polygon with id '" << _id
					  << "' already exists" << std::endl;
			return (true);
		}

		if (!poly.isNear(*this))
			return (false);

		points = poly.getPoints();
		for (Point &point : *(this->getPoints())) {

			if (point.isOnPolyBorder(poly))
				continue ;
			if (point.isInPoly(poly)) {

				std::cerr << "ERROR HANDLING POLYGON: received polygon has "
						  << "intersections with present map" << std::endl;
				return (true);
			}
		}
		for (Point &point : *(poly.getPoints())) {

			if (point.isOnPolyBorder(*this))
				continue ;
			if (point.isInPoly(*this)) {

				std::cerr << "ERROR HANDLING POLYGON: received polygon has "
						  << "intersections with present map" << std::endl;
				return (true);
			}
		}
		return (false);
	}
}
