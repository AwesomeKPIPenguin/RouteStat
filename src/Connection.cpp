
#include "Connection.hpp"

namespace RouteStat {

	const std::string Connection::PROTOCOL = "tcp";

	Connection::Connection(std::string host, std::string pub, std::string sub)
		: _host(std::move(host)),
		  _pub(std::move(pub)),
		  _sub(std::move(sub)),
		  _context(zmq::context_t(1)),
		  _pubs(zmq::socket_t(_context, ZMQ_PUB)),
		  _subs(zmq::socket_t(_context, ZMQ_SUB)) {

		_pubs.bind(PROTOCOL + "://" + _host + ":" + _pub);
		_subs.connect(PROTOCOL + "://" + _host + ":" + _sub);
		usleep(100);
		if (!_subs.connected())
			std::cout << "CONNECTION ERROR: unable to connect to ["
					  << PROTOCOL << "://" << _host << ":" << _sub << "]"
					  << std::endl;
	}
}
