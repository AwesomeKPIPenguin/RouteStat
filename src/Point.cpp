
#include "Point.hpp"
#include "Polygon.hpp"

namespace RouteStat {

	// if it is more then about 1000 points in polygon, it is efficient to
	//   use multiple threads (in general, 8 is enough)

	const int		Point::START_THREADING = 1000;
	const int		Point::THREADS = 8;

	// latitude and longitude are between -90 and 90, -180 and 180

	const double	Point::NULL_VALUE = 181;



	Point::Point() = default;

	Point::Point(double la, double lo)
		: _la(la), _lo(lo) { }

	double			Point::getLa() const { return (_la); }
	double			Point::getLo() const { return _lo; }

	void			Point::setLa(double la) { Point::_la = la; }
	void			Point::setLo(double lo) { Point::_lo = lo; }

	Point &			Point::operator=(const RouteStat::Point &copy) {

		this->_la = copy._la;
		this->_lo = copy._lo;
		return (*this);
	}

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

	void			Point::isInPolyPart(
						const Polygon & poly, int from, int to, bool * res) {

		std::vector<Point> *	points;

		*res = false;

		points = poly.getPoints();
		for (int i = from + 1, j = from; i < to; j = i++) {

			if ((((*points)[i]._lo > _lo) != ((*points)[j]._lo > _lo))
			&& (_la < ((*points)[j]._la - (*points)[i]._la) * (_lo - (*points)[i]._lo)
				/ ((*points)[j]._lo - (*points)[i]._lo) + (*points)[i]._la))
			*res = !*res;
		}
	}

	bool			Point::isInPolyFull(const Polygon & poly) {

		bool					res;
		std::vector<Point> *	points;

		res = false;

		points = poly.getPoints();
		for (int i = 0, j = points->size() - 1; i < points->size(); j = i++) {

			if ((((*points)[i]._lo > _lo) != ((*points)[j]._lo > _lo))
			&& (((*points)[j]._la - (*points)[i]._la) * (_lo - (*points)[i]._lo)
				/ ((*points)[j]._lo - (*points)[i]._lo) + (*points)[i]._la - _la <= PREC))
			res = !res;
		}
		return (res);
	}

	bool			Point::isInPoly(const Polygon & poly) {

		std::thread		threads[THREADS];
		bool			res[THREADS];
		unsigned long	len;
		int				part;
		int				to;

		if (!isNearPoly(poly))
			return (false);

		len = poly.getPoints()->size();
		if (len > START_THREADING) {

			part = len / THREADS;
			for (int i = 0; i < THREADS; ++i) {

				to = (i == THREADS - 1) ? len : (i + 1) * part;
				threads[i] = std::thread([&]() {
					isInPolyPart(poly, i * part, to, &res[i]);
				});
			}
			for (int i = 0; i < THREADS; ++i)
				threads[i].join();
			for (bool b : res) {
				if (!b) return (false);
			}
			return (true);
		}
		else {
			return (isInPolyFull(poly));
		}
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

	void			Point::segmentPolyPartIntersection(
						int i, Point &p1, Point &p2,
						Polygon &poly, Point &res) {

		std::vector<Point>	*points;

		points = poly.getPoints();
		for (int j = i; j < points->size() - 1; j += THREADS) {

			if (segmentSegmentIntersection(
				p1, p2, (*points)[j], (*points)[j + 1], res))
				return ;
		}
	}

	bool			Point::segmentPolyIntersection(
						Point &p1, Point &p2, Polygon &poly, Point &res) {

		std::vector<Point>	*points;
		std::thread			threads[THREADS];
		Point				results[THREADS];

		if (!p1.isNearPoly(poly) && !p2.isNearPoly(poly))
			return (false);

		res.setLa(NULL_VALUE);
		points = poly.getPoints();
		for (Point &p : results)
			p.setLa(NULL_VALUE);
		if (points->size() < START_THREADING) {

			for (int i = 0, j = points->size() - 1; i < points->size(); j = i++) {

				std::cout << p1 << "-" << p2 << " X " << (*points)[i] << "-" << (*points)[j] << " -> ";
				if (segmentSegmentIntersection(
					p1, p2, (*points)[i], (*points)[j], res)) {
					std::cout << res << std::endl;
					return (true);
				}
				else
					std::cout << "null" << std::endl;
			}
		}
		else {

			for (int i = 0; i < THREADS; ++i) {
				threads[i] = std::thread([&]() {
					segmentPolyPartIntersection(i, p1, p2, poly, results[i]);
				});
			}
			for (auto &t : threads)
				t.join();
			for (Point &p : results) {

				if (p.getLa() != NULL_VALUE) {

					res = p;
					return (true);
				}
			}
		}
		return (false);
	}
}