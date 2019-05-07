
#pragma once

#include <iostream>
#include <cmath>

namespace routeStat {

	class Polygon;

	class Point {

	private:

		double					_la;
		double					_lo;

	public:

		static constexpr double	PREC = 1e-9;

		Point();
		Point(double, double);

		// getters/setters

		double					getLa() const;
		double					getLo() const;

		void					setLa(double);
		void					setLo(double);

		// operators

		friend Point			operator+(const Point &, const Point &);
		friend Point			operator-(const Point &, const Point &);
		friend Point			operator*(const Point &, const double);
		friend Point			operator*(const double, const Point &);
		friend std::ostream &	operator<<(std::ostream &, const Point &);

		// geometry

		double					len() const;
		double					dot(const Point &) const;
		double					getDist(const Point &) const;
		bool					isInLine(const Point &, const Point &) const;
		bool					isNearPoly(const Polygon &) const;

		static bool				isInPoly(const Point &, const Polygon &);
		static bool				segmentSegmentIntersection(
									Point &, Point &, Point &, Point &, Point &);
	};
}
