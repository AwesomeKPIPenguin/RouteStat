
#include "Polygon.hpp"
#include "Connection.hpp"
#include "DB.hpp"

using namespace std;
using namespace RouteStat;

	// av: host, pub port, sub port, db host, db port, db name, db user, db password
	// test example: 127.0.0.1 5000 6000 127.0.0.1 5432 map postgres postgres

int main(int ac, char **av)
{
	vector<Polygon> map;

	if (ac != 9) {

		cerr << "Wrong number of arguments" << endl;
		return (1);
	}

	Connection		conn(av[1], av[2], av[3]);
	DB				db(av[4], av[5], av[6], av[7], av[8]);

	map.reserve(10000);

	db.initMap(&map);
	conn.listen(db, &map);

	return (0);
}
