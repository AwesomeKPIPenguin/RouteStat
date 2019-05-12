
#pragma once

#include <iostream>
#include <cmath>
#include <thread>

namespace RouteStat {

	class Polygon;

	class Point {

	private:

		double					_la;
		double					_lo;

		void					isInPolyPart(const Polygon &, int, int, bool *);
		bool					isInPolyFull(const Polygon &);

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
		bool					isInPoly(const Polygon &);

		static bool				segmentSegmentIntersection(
									Point &, Point &, Point &, Point &, Point &);
	};
}
