
#include "Connection.hpp"

namespace RouteStat {

	const std::string	Connection::PROTOCOL = "tcp";

	void		Connection::checkConnections() {

		if (_subs.connected())
			std::cout << "Successfully subscribed to             ["
					  << PROTOCOL << "://" << _host << ":" << _sub << "]"
					  << std::endl;
		else
			std::cout << "CONNECTION ERROR: unable to subscribe to ["
					  << PROTOCOL << "://" << _host << ":" << _sub << "]"
					  << std::endl;
		if (_pubs.connected())
			std::cout << "Successfully binded publisher to       ["
					  << PROTOCOL << "://" << _host << ":" << _pub << "]"
					  << std::endl;
		else
			std::cout << "CONNECTION ERROR: unable to bind publisher to ["
					  << PROTOCOL << "://" << _host << ":" << _pub << "]"
					  << std::endl;
	}

	Connection::Connection(std::string host, std::string pub, std::string sub)
		: _host(std::move(host)),
		  _pub(std::move(pub)),
		  _sub(std::move(sub)),
		  _context(zmq::context_t(1)),
		  _pubs(zmq::socket_t(_context, ZMQ_PUB)),
		  _subs(zmq::socket_t(_context, ZMQ_SUB)),
		  _res("[]"_json) {

		_pubs.bind(PROTOCOL + "://" + _host + ":" + _pub);
		_subs.connect(PROTOCOL + "://" + _host + ":" + _sub);
		_subs.setsockopt(ZMQ_SUBSCRIBE, "", 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		checkConnections();
	}

	void		Connection::send() {

		zmq::message_t	msg;
		std::string		str;

		str = _res.dump();
		msg = zmq::message_t(str.c_str(), str.length());
		_pubs.send(msg);
	}

	void		Connection::listenForExit() {

		std::string		input;

		while (true) {

			std::cin >> input;
			if (input == "exit")
				exit(EXIT_SUCCESS);
		}
	}

	void		Connection::listen(DB &db, std::vector<Polygon> *map) {

		std::thread		thread;
		zmq::message_t	msg;
		json			json;

		thread = std::thread([&]() { listenForExit(); });
		while (true) {

			_subs.recv(&msg);
			json = nlohmann::json::parse((char *)msg.data());
			if (json[0].is_array())
				handleRoute(map, json);
			else
				handlePolygon(db, map, json);
		}
	}

}
