
#include <fstream>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <thread>

#include "Polygon.hpp"

using namespace std;
using namespace RouteStat;
using hclock = std::chrono::high_resolution_clock;
using microseconds = std::chrono::microseconds;

//int main() {
//
//	json j = "["
//		"[0, 0],"
//		"[-3, 1],"
//		"[0, 2],"
//		"[1, 5],"
//		"[2, 2],"
//		"[5, 1],"
//		"[2, 0],"
//		"[1, -3]"
//		"]"_json;
//
//
//	auto poly = Polygon(j);
//	vector<Point> points = vector<Point>();
//
//	for (int i = 0; i < 100; ++i)
//		points.emplace_back(Point((double)i * 0.1 - 4, (double)i * 0.1 - 4));
//
//	auto point = Point(1, -1.5);
//	cout << point << ": " << point.isInPoly(poly) << endl;
//
////	cout << "Min Latitude:  " << poly.getMinLa() << endl
////		 << "Max Latitude:  " << poly.getMaxLa() << endl
////		 << "Min Longitude: " << poly.getMinLo() << endl
////		 << "Max Longitude: " << poly.getMaxLo() << endl;
////	cout << endl << "Points:" << endl;
////	for (Point p : poly.getPoints())
////		cout << "  " << p << endl;
//
//
//	return (0);
//}