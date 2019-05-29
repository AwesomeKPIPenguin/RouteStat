
#include <json.hpp>
#include "DB.hpp"

namespace RouteStat {

	const std::string	DB::TABLE = "polygons";
	const std::string	DB::IS_TABLE_EXISTS = "SELECT to_regclass('" + TABLE + "');";
	const std::string	DB::CREATE_TABLE = "CREATE TABLE " + TABLE + " ("\
	"id INTEGER PRIMARY KEY NOT NULL,"\
	"points JSON NOT NULL);";
	const std::string	DB::SELECT = "SELECT * FROM " + TABLE + ";";
	const std::string	DB::INSERT = "INSERT INTO " + TABLE + " (id, points)"\
									 "VALUES($1, $2);";

	DB::DB(
		std::string host,
		std::string port,
		std::string name,
		std::string user,
		std::string pass
	) :
	_host(std::move(host)),
	_port(std::move(port)),
	_name(std::move(name)),
	_user(std::move(user)),
	_pass(std::move(pass)),
	_conn(
		"dbname = " + _name
		+ " user = " + _user
		+ " password = " + _pass
		+ " hostaddr = " + _host
		+ " port = " + _port
	) {

		if (_conn.is_open())
			std::cout << "Successfully connected to database     ["
					  << _host << ":" << _port << "/" << _name << "]"
					  << std::endl << std::endl;
		else
			std::cout << "DATABASE CONNECTION ERROR: failed to connect "
					  << "to database [" << _host << ":" << _port << "/"
					  << _name << "]" << std::endl;
	}

	std::string	DB::getHost() const { return (_host); }
	std::string	DB::getPort() const { return (_port); }
	std::string	DB::getName() const { return (_name); }
	std::string	DB::getUser() const { return (_user); }
	std::string	DB::getPass() const { return (_pass); }



	void		DB::initTable() {

		pqxx::work		w(_conn);
		pqxx::result	res;

		res = w.exec(IS_TABLE_EXISTS);

		try {
			res.begin()[0].as<std::string>();
		} catch (const std::exception &e) {

			w.exec(CREATE_TABLE);

			std::cout << "Successfully created table '" << TABLE
					  << "'" << std::endl;
		}
		w.commit();
	}

	void		DB::initMap(std::vector<Polygon> *map) {


		pqxx::result					res;
		pqxx::result::const_iterator	record;

		initTable();
		pqxx::work w(_conn);
		try {
			res = w.exec(SELECT);
			w.commit();
		} catch (const std::exception &e) {
			w.abort();
			throw;
		}
		for (record = res.begin(); record != res.end(); ++record)
			map->emplace_back(
				record[0].as<int>(), nlohmann::json::parse(
					record[1].as<std::string>()));
	}

	void		DB::insert(int id, nlohmann::json points) {

		pqxx::nontransaction	t(_conn);

		_conn.prepare("insert", INSERT);
		t.prepared("insert")(id)(points.dump()).exec();
		t.commit();
	}
}
