
#include <fstream>
#include <iostream>
#include <json.hpp>

using namespace std;
using namespace nlohmann;

int		main() {

	ofstream	out;
	ifstream	in;
	string		str;
	json		j;
	json		res;

	in.open("track_example.txt");
	in >> str;
	for (char &c : str)
		if (c == '\'')
			c = '"';
	j = json::parse(str);
	for (auto &arr : j)
		res.emplace_back(json::array({arr[0], arr[1]}));
	cout << res.dump() << endl;
	out.open("track_points.txt");
	out << res.dump();

	return (0);
}