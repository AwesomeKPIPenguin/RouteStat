
#include "Handlers.hpp"

namespace RouteStat {

	json	Handlers::_res = "[]"_json;



	json	&Handlers::getRes() { return (_res); }

	void	Handlers::flush() { _res = "[]"_json; }



	void	Handlers::addPoint(
				RoutePoint p1, RoutePoint p2,
				Point &inter, int id, const std::string &position) {

		double	dist;
		double	len;
		char	timeStr[10];
		time_t	t[3];

		bzero(timeStr, 10);
		if (position == " ") {

			strftime(timeStr, 10, "%H:%M:%S", p1.getTime());
			_res.emplace_back(
				json::array({
					inter.getLa(),
					inter.getLo(),
					p1.getDist(),
					timeStr,
					p1.getDuration(),
					(id >= 0) ? std::to_string(id) : "",
					position
				})
			);
			return ;
		}
		len = p1.getPoint().getDist(p2.getPoint());
		dist = p1.getPoint().getDist(inter);
		t[0] = mktime(p1.getTime());
		t[1] = mktime(p2.getTime());
		t[2] = (time_t)(t[0] + difftime(t[1], t[0]) * dist / len);
		strftime(timeStr, 10, "%H:%M:%S", localtime(&t[2]));
		_res.emplace_back(
			json::array({
				inter.getLa(),
				inter.getLo(),
				(int)(dist * 10e5),
				timeStr,
				(int)(p1.getDuration() + difftime(t[2], t[0])),
				(id >= 0) ? std::to_string(id) : "",
				position
			})
		);
	}

	bool	Handlers::handlePolygon(std::vector <Polygon> *map, json &json) {

        Polygon	poly;

        poly = Polygon(
            std::stoi(json[0]["name"].get<std::string>()),
            json[0]["coords"][0]);

        for (Polygon p : *map)
            if (poly.isInterPoly(p))
                return (false);

        map->emplace_back(poly);
        return (true);
    }

    Polygon	*Handlers::handleRouteEndPoint(
				std::vector <Polygon> *map, RoutePoint &rp) {

        Polygon	*res;

        res = nullptr;
        for (Polygon &poly : *map) {

            if (rp.getPoint().isInPoly(poly)) {

                res = &poly;
                break;
            }
        }
        addPoint(
        	rp, rp, rp.getPoint(), (res) ? res->getId() : -1, " ");
        return (res);
    }

    void	Handlers::handleRouteInside(
				RoutePoint &p1, RoutePoint &p2,
				Polygon **cp, std::vector <Polygon> *map) {

        bool	foundNeighbor;
        Point	res;

        foundNeighbor = false;
        if (Point::segmentPolyIntersection(
                p1.getPoint(), p2.getPoint(), **cp, res)) {

            addPoint(p1, p2, res, (*cp)->getId(), "OUT");
            for (Polygon &poly : *map) {

                if (&poly == (*cp))
                    continue;
                if (p2.getPoint().isInPoly(poly)) {

                    (*cp) = &poly;
                    foundNeighbor = true;
                    addPoint(p1, p2, res, (*cp)->getId(), "IN");
                    break;
                }
            }
            if (!foundNeighbor)
                *cp = nullptr;
        }
    }

    void	Handlers::handleRouteOutside(
				RoutePoint &p1, RoutePoint &p2,
				Polygon **cp, std::vector <Polygon> *map) {

        Point	res;

        for (Polygon &poly : *map) {

            if (!p1.getPoint().isNearPoly(poly)
                && !p2.getPoint().isNearPoly(poly))
                continue;

            if (Point::segmentPolyIntersection(
                    p1.getPoint(), p2.getPoint(), poly, res)) {

                *cp = &poly;
                addPoint(p1, p2, res, (*cp)->getId(), "IN");
                break;
			}
        }
    }

    void	Handlers::handleRoute(std::vector <Polygon> *map, json &json) {

        Polygon		*currPoly;
        RoutePoint	segment[2];

        std::cout << "Accepted new route" << std::endl;

        currPoly = nullptr;
        segment[0] = RoutePoint(json[0]);
        segment[1] = segment[0];

        currPoly = handleRouteEndPoint(map, segment[0]);
        for (int i = 1; i < json.size(); ++i) {

            segment[0] = segment[1];
            segment[1] = RoutePoint(json[i]);
            if (currPoly)
                handleRouteInside(segment[0], segment[1], &currPoly, map);
            else
                handleRouteOutside(segment[0], segment[1], &currPoly, map);
        }
        handleRouteEndPoint(map, segment[1]);
    }
}
