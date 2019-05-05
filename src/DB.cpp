
#include "DB.hpp"

namespace routeStat {

	DB::DB() = default;

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
	_pass(std::move(pass)) {

//		_conn = init_conn(_host, _port, _name,  _user, _pass ...);
	}

	std::string	DB::getHost() const { return (_host); }
	std::string	DB::getPort() const { return (_port); }
	std::string	DB::getName() const { return (_name); }
	std::string	DB::getUser() const { return (_user); }
	std::string	DB::getPass() const { return (_pass); }


}
