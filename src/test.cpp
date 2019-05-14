
#include <fstream>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <thread>
#include <iostream>
#include <cmath>

using namespace std;

int main() {

	char	buff[10];
	time_t	time;

	time = chrono::system_clock::to_time_t(chrono::system_clock::now());
	tm tm = *localtime(&time);
	strftime(buff, 10, "%H:%M:%S", &tm);
	cout << buff << endl;

	tm.tm_sec += 180;
	mktime(&tm);
	strftime(buff, 10, "%H:%M:%S", &tm);
	cout << buff << endl;

	return (0);
}