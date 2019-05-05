
#pragma once

#include <string>

namespace routeStat {

	class DB
	{
	private:

		std::string	_host;
		std::string	_port;
		std::string	_name;
		std::string	_user;
		std::string	_pass;
//		CONN		_conn;

		DB();

	public:

		DB(std::string, std::string, std::string, std::string, std::string);

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
