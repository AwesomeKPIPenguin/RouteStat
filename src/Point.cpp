
#include "Point.hpp"

Point::Point() = default;

Point::Point(double _lat, double _long)
	: _lat(_lat), _long(_long) { }

double	Point::getLat() const { return (_lat); }
double	Point::getLong() const { return _long; }

void	Point::setLat(double _lat) { Point::_lat = _lat; }
void	Point::setLong(double _long) { Point::_long = _long; }

Point	operator+(const Point &p1, const Point &p2) {
	return (Point(p1._lat + p2._lat, p1._long + p2._long));
}

Point	operator-(const Point &p1, const Point &p2) {
	return (Point(p1._lat - p2._lat, p1._long - p2._long));
}

Point	operator*(const double k, const Point &p) {
	return (Point(p._lat * k, p._long * k));
}

Point	operator*(const Point &p, const double k) {
	return (Point(p._lat * k, p._long * k));
}

std::ostream &	operator<<(std::ostream &os, const Point &p) {
	return (os << "(" << p._lat << ", " << p._long << ")");
}



double	Point::getDist(const Point &p) const {
	return (sqrt(pow(p._lat - _lat, 2.0) + pow(p._long - _long, 2.0)));
}

bool	Point::isInLine(const Point &p1, const Point &p2) const {
	return ((p1.getDist(p2) + p2.getDist(*this) - p1.getDist(*this)) < PREC);
}

bool	Point::segmentSegmentIntersection(
	Point &p11, Point &p12, Point &p21, Point &p22, Point &inter) {

	Point	vec1;
	Point	vec2;
	double	d1;
	double	d2;
	double	side[4];

	vec1 = p12 - p11;
	d1 = -(-vec1._long * p11._lat + vec1._lat * p11._long);
	side[0] = -vec1._long * p21._lat + vec1._lat * p21._long + d1;
	side[1] = -vec1._long * p22._lat + vec1._lat * p22._long + d1;
	if (side[0] * side[1] > 0)
		return (false);

	vec2 = p22 - p21;
	d2 = -(-vec2._long * p21._lat + vec2._lat * p21._long);
	side[2] = -vec2._long * p11._lat + vec2._lat * p11._long + d2;
	side[3] = -vec2._long * p12._lat + vec2._lat * p12._long + d2;
	if (side[2] * side[3] > 0)
		return (false);

	inter = p11 + (side[2] / (side[2] - side[3])) * vec1;
	return (true);
}