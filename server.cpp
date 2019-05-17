
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
	json		poly[4];
	string		str;

	pubs.bind("tcp://127.0.0.1:6000");
	subs.connect("tcp://127.0.0.1:5000");
	subs.setsockopt(ZMQ_SUBSCRIBE, "", 0);

	sleep(5);

//	poly[0] = "["\
//		"{"\
//			"\"color\": \"#ff0000\","\
//			"\"coords\": [["\
//				"[2, 1],"\
//				"[1, 2],"\
//				"[2, 3],"\
//				"[3, 2]"\
//			"]],"\
//			"\"name\": \"001\""\
//		"}"
//	"]"_json;
//
//	str = poly[0].dump();
//	msg = message_t(str.c_str(), str.size());
//	pubs.send(msg);
//
//	poly[1] = "["\
//		"{"\
//			"\"color\": \"#ff0000\","\
//			"\"coords\": [["\
//				"[4, 3],"\
//				"[3, 4],"\
//				"[3, 5],"\
//				"[5, 5],"\
//				"[5, 3]"\
//			"]],"\
//			"\"name\": \"002\""\
//		"}"
//	"]"_json;
//
//	str = poly[1].dump();
//	msg = message_t(str.c_str(), str.size());
//	pubs.send(msg);

//	poly[2] = "["\
//		"{"\
//			"\"color\": \"#ff0000\","\
//			"\"coords\": [["\
//				"[5, 3],"\
//				"[5, 5],"\
//				"[7, 5],"\
//				"[7, 3]"\
//			"]],"\
//			"\"name\": \"003\""\
//		"}"
//	"]"_json;
//
//	str = poly[2].dump();
//	msg = message_t(str.c_str(), str.size());
//	pubs.send(msg);

//	poly[3] = "["\
//		"{"\
//			"\"color\": \"#ff0000\","\
//			"\"coords\": [["\
//				"[8, 3],"\
//				"[8, 5],"\
//				"[10, 5],"\
//				"[10, 3]"\
//			"]],"\
//			"\"name\": \"004\""\
//		"}"
//	"]"_json;
//
//	str = poly[3].dump();
//	msg = message_t(str.c_str(), str.size());
//	pubs.send(msg);
//
	str = "["\
		"[2,   2,   0, \"14:33:21\", 52401],"\
		"[3.1, 2.9, 0, \"14:33:24\", 52404],"\
		"[4,   4,   0, \"14:33:25\", 52405],"\
		"[6,   4.1, 0, \"14:33:26\", 52406],"\
		"[7.5, 4,   0, \"14:34:21\", 52461],"\
		"[9,   4,   0, \"14:34:24\", 52464]"\
	"]\0";

	msg = message_t(str.c_str(), str.length() + 1);
	pubs.send(msg);

	subs.recv(&msg);
	cout << "Received back:" << endl << (char *)msg.data() << endl;
}
