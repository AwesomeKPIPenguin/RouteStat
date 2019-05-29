# RouteStat

Application for finding intersections of route (polyline) with map of some areas (polygons).

## Installation and Usage

Application uses sockets for communication (**ZeroMQ** pub/sub) - it receives and responds data in json format (example files included).
Received polygons are stored in specified DB (**PostgreSQL** DBMS)

There is a configurational script for installing all necessary libs:

```
sh config.sh
```

There are **Makefile** and **CMakeLists.txt** files for building.

All connection parameters pass through command line arguments:

```
./routestat host pub_port sub_port db_host db_port db_name db_user db_password
```

Example:

```
./routestat 127.0.0.1 5000 6000 127.0.0.1 5432 map postgres postgres
```

To exit type 'exit'.
