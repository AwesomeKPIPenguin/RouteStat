
#include <zmq.hpp>
#include <json.hpp>
#include <zconf.h>

using namespace zmq;
using namespace nlohmann;

int		main() {

	context_t	context(1);
	socket_t	pubs(context, ZMQ_PUB);
	socket_t	subs(context, ZMQ_SUB);
	message_t	msg;
	json		points;

	pubs.bind("tcp://127.0.0.1:6000");
	subs.connect("tcp://127.0.0.1:5000");

	points = "["\
		"{"\
			"\"color\": \"#ff0000\","\
			"\"coords\": [["\
				"[1, 1],"\
				"[1, 2],"\
				"[2, 2],"\
				"[2, 1]"\
			"]],"\
			"\"name\": \"001\""\
		"}"
	"]"_json;

	auto str = points.dump();
	msg = message_t(str.c_str(), str.size());

	sleep(5);
	pubs.send(msg);
}
