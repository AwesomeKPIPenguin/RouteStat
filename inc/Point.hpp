
#pragma once

#include <iostream>
#include <cmath>
#include <thread>
#include <atomic>

namespace RouteStat {

	class Polygon;

	class Point {

	private:

		static const int		START_THREADING;
		static const int		THREADS;
		static const double		NULL_VALUE;

		double					_la;
		double					_lo;

		void					isInPolyPart(
									const Polygon &poly, int i, bool *res) const;
		bool					isInPolyFull(const Polygon &poly) const;

		static void				segmentPolyPartIntersection(
									int i, Point &p1, Point &p2,
									Polygon &poly, Point &res);
		static bool				segmentSegmentIntersection(
									Point &, Point &, Point &, Point &, Point &);
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

		Point &					operator=(const Point &);
		friend Point			operator+(const Point &, const Point &);
		friend Point			operator-(const Point &, const Point &);
		friend Point			operator*(const Point &, const double);
		friend Point			operator*(const double, const Point &);
		friend std::ostream &	operator<<(std::ostream &, const Point &);

		// geometry

		double					len() const;
		double					dot(const Point &p) const;
		double					getDist(const Point &p) const;
		bool					isInLine(const Point &p1, const Point &p2) const;
		bool					isNearPoly(const Polygon &poly) const;
		bool					isInPoly(const Polygon &poly) const;
		bool					isOnPolyBorder(const Polygon &poly) const;

		static bool				segmentPolyIntersection(
									Point &, Point &, Polygon &, Point &);
	};
}
