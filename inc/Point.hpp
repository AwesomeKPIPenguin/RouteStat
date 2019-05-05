
#pragma once

#include <iostream>
#include <cmath>

class Point {

private:

	double					_lat;
	double					_long;

public:

	static constexpr double	PREC = 1e-9;

	Point();
	Point(double _lat, double _long);

	// getters/setters

	double					getLat() const;
	double					getLong() const;

	void					setLat(double _lat);
	void					setLong(double _long);

	// operators

	friend Point			operator+(const Point &, const Point &);
	friend Point			operator-(const Point &, const Point &);
	friend Point			operator*(const Point &, const double);
	friend Point			operator*(const double, const Point &);
	friend std::ostream &	operator<<(std::ostream &, const Point &);

	// geometry

	double					getDist(const Point &) const;
	bool					isInLine(const Point &, const Point &) const;
	static bool				segmentSegmentIntersection(
								Point &, Point &, Point &, Point &, Point &);
};
