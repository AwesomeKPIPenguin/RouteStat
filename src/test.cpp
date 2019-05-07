
#include <fstream>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <thread>

#include "Polygon.hpp"

using namespace std;
using namespace routeStat;
using hclock = std::chrono::high_resolution_clock;
using microseconds = std::chrono::microseconds;

static const int	THREADS = 8;


// the num is 1000


void	test1Threads(const Polygon & poly) {

	Point	point;

	point = Point(2, 2);
	for (int i = 0; i < 1000; ++i) {

		auto start = hclock::now();

		for (int j = 0; j < THREADS * i; ++j)
			Point::isInPoly(point, poly);

		cout << std::setw(5) << i * THREADS << " points ONE thread: " << std::chrono::duration_cast<microseconds>(hclock::now() - start).count() << " mcs" << endl;
	}
}

void	threadFt(const Point & p, const Polygon & poly, int i) {

	for (int j = 0; j < i; ++j)
		Point::isInPoly(p, poly);
}

void	test8Threads(const Polygon & poly) {

	Point	point;

	point = Point(2, 2);
	for (int i = 0; i < 1000; ++i) {

		std::thread	threads[THREADS];

		auto start = hclock::now();

		for (int k = 0; k < THREADS; ++k)
			threads[k] = std::thread(threadFt, point, poly, i);
		for (int k = 0; k < THREADS; ++k)
			threads[k].join();

		cout << std::setw(5) << i * THREADS << " points EIGHT threads: " << std::chrono::duration_cast<microseconds>(hclock::now() - start).count() << " mcs" << endl;
	}
}

int main() {

	string	data;
	json j = "["
		"[1, 0],"
		"[0, 1],"
		"[-0.5, 2],"
		"[0, 3],"
		"[1, 4],"
		"[3, 4],"
		"[4, 3],"
		"[4.5, 2],"
		"[4, 1],"
		"[3, 0]"
		"]"_json;


	Polygon poly = Polygon(j);

//	cout << "Min Latitude:  " << poly.getMinLa() << endl
//		 << "Max Latitude:  " << poly.getMaxLa() << endl
//		 << "Min Longitude: " << poly.getMinLo() << endl
//		 << "Max Longitude: " << poly.getMaxLo() << endl;
//	cout << endl << "Points:" << endl;
//	for (Point p : poly.getPoints())
//		cout << "  " << p << endl;

	test1Threads(poly);
	test8Threads(poly);

	return (0);
}