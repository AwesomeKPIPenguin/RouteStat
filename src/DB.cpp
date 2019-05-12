
#include "DB.hpp"

namespace RouteStat {

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
	),
	_work(_conn) {

		if (_conn.is_open())
			std::cout << "Connected to database '" << _name << "'" << std::endl;
		else
			std::cout << "Failed to connect to database '" << _name << "'" << std::endl;
	}

	std::string	DB::getHost() const { return (_host); }
	std::string	DB::getPort() const { return (_port); }
	std::string	DB::getName() const { return (_name); }
	std::string	DB::getUser() const { return (_user); }
	std::string	DB::getPass() const { return (_pass); }
}
