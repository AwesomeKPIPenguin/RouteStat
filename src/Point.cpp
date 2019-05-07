
#include "Point.hpp"
#include "Polygon.hpp"

namespace routeStat {

	Point::Point() = default;

	Point::Point(double la, double lo)
		: _la(la), _lo(lo) { }

	double			Point::getLa() const { return (_la); }
	double			Point::getLo() const { return _lo; }

	void			Point::setLa(double la) { Point::_la = la; }
	void			Point::setLo(double lo) { Point::_lo = lo; }

	Point			operator+(const Point &p1, const Point &p2) {
		return (Point(p1._la + p2._la, p1._lo + p2._lo));
	}

	Point			operator-(const Point &p1, const Point &p2) {
		return (Point(p1._la - p2._la, p1._lo - p2._lo));
	}

	Point			operator*(const double k, const Point &p) {
		return (Point(p._la * k, p._lo * k));
	}

	Point			operator*(const Point &p, const double k) {
		return (Point(p._la * k, p._lo * k));
	}

	std::ostream &	operator<<(std::ostream &os, const Point &p) {
		return (os << "(" << p._la << ", " << p._lo << ")");
	}



	double			Point::len() const {
		return (sqrt(pow(_la, 2.0) + pow(_lo, 2.0)));
	}

	double			Point::dot(const Point & p) const {
		return (this->_la * p._la + this->_lo * p._lo);
	}

	double			Point::getDist(const Point &p) const {
		return ((p - *this).len());
	}

	bool			Point::isInLine(const Point &p1, const Point &p2) const {
		return ((p1.getDist(p2) + p2.getDist(*this) - p1.getDist(*this)) < PREC);
	}

	bool			Point::isNearPoly(const Polygon & poly) const {
		return (_la >= poly.getMinLa() && _la <= poly.getMaxLa()
			 && _lo >= poly.getMinLo() && _lo <= poly.getMaxLo());
	}

	bool			Point::isInPoly(const Point & p, const Polygon & poly) {

		bool				c;
		std::vector<Point>	points;

		if (!p.isNearPoly(poly))
			return (false);

		c = false;
		points = poly.getPoints();
		for (int i = 0, j = points.size() - 1; i < points.size(); j = i++) {

			if (((points[i]._lo > p._lo) != (points[j]._lo > p._lo))
			&& (p._la < (points[j]._la - points[i]._la) * (p._lo - points[i]._lo)
				/ (points[j]._lo - points[i]._lo) + points[i]._la))
			c = !c;
		}
		return (c);
	}

	bool			Point::segmentSegmentIntersection(
						Point & p11,
						Point & p12,
						Point & p21,
						Point & p22,
						Point & inter) {

		Point	vec1;
		Point	vec2;
		double	d1;
		double	d2;
		double	side[4];

		vec1 = p12 - p11;
		d1 = -(-vec1._lo * p11._la + vec1._la * p11._lo);
		side[0] = -vec1._lo * p21._la + vec1._la * p21._lo + d1;
		side[1] = -vec1._lo * p22._la + vec1._la * p22._lo + d1;
		if (side[0] * side[1] > 0)
			return (false);

		vec2 = p22 - p21;
		d2 = -(-vec2._lo * p21._la + vec2._la * p21._lo);
		side[2] = -vec2._lo * p11._la + vec2._la * p11._lo + d2;
		side[3] = -vec2._lo * p12._la + vec2._la * p12._lo + d2;
		if (side[2] * side[3] > 0)
			return (false);

		inter = p11 + (side[2] / (side[2] - side[3])) * vec1;
		return (true);
	}
}