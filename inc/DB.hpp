
#pragma once

#include <iostream>
#include <string>
#include <pqxx/pqxx>

namespace RouteStat {

	class DB
	{
	private:

		std::string			_host;
		std::string			_port;
		std::string			_name;
		std::string			_user;
		std::string			_pass;
		pqxx::connection	_conn;
		pqxx::work			_work;

	public:

		DB(
			std::string host,
			std::string port,
			std::string name,
			std::string user,
			std::string pass
		);

		// getters

		std::string	getHost() const;
		std::string	getPort() const;
		std::string	getName() const;
		std::string	getUser() const;
		std::string	getPass() const;

		// logic

//		void		initMap(Map &) const;
	};
}
