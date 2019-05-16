
#include <iostream>
#include <zmq.hpp>
#include <json.hpp>
#include <zconf.h>

using namespace std;
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
	subs.setsockopt(ZMQ_SUBSCRIBE, "", 0);

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

	str = "["\
		"[1,  -1,   0, \"14:33:21\", 52401],"\
		"[1.5, 1.5, 4, \"14:33:24\", 52404],"\
		"[2,   4,   2, \"14:33:25\", 52405]"\
	"]\0";

	msg = message_t(str.c_str(), str.length() + 1);
	pubs.send(msg);

	subs.recv(&msg);
	cout << "Received back:" << endl << (char *)msg.data() << endl;
}
