
#pragma once

#include "Polygon.hpp"
#include "RoutePoint.hpp"

namespace RouteStat {

    class Handlers {

	private:

		static json		_res;

		static void		addPoint(
							RoutePoint p1, RoutePoint p2, Point &inter,
							int id, const std::string &position);
		static Polygon	*handleRouteEndPoint(
							std::vector <Polygon> *map, RoutePoint &rp);
		static void		handleRouteInside(
							RoutePoint &p1, RoutePoint &p2,
							Polygon **cp, std::vector <Polygon> *map);
		static void		handleRouteOutside(
							RoutePoint &p1, RoutePoint &p2,
							Polygon **cp, std::vector <Polygon> *map);

	public:

    	static json		&getRes();
    	static void		flush();

		static bool		handlePolygon(std::vector <Polygon> *map, json &json);
		static void		handleRoute(std::vector <Polygon> *map, json &json);
    };
}
