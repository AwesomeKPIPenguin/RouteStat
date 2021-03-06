
#pragma once

#include <iostream>
#include <string>
#include <pqxx/pqxx>

#include "Polygon.hpp"

namespace RouteStat {

	class DB
	{
	private:

		static const std::string	TABLE;
		static const std::string	IS_TABLE_EXISTS;
		static const std::string	CREATE_TABLE;
		static const std::string	SELECT;
		static const std::string	INSERT;

		std::string			_host;
		std::string			_port;
		std::string			_name;
		std::string			_user;
		std::string			_pass;
		pqxx::connection	_conn;

		void				initTable();

	public:

		DB(
			std::string host,
			std::string port,
			std::string name,
			std::string user,
			std::string pass
		);

		// getters

		std::string			getHost() const;
		std::string			getPort() const;
		std::string			getName() const;
		std::string			getUser() const;
		std::string			getPass() const;

		// logic

		void				initMap(std::vector<Polygon> *);
		void				insert(int, nlohmann::json);
	};
}
