#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <string.h>
#include <time.h>
#include "Request.h"
#include <iostream>
#include <sstream>


void run()
{
	ostringstream bufferStream;
	string tmp;

	bufferStream << "GET /WEBSITE.HTML?lang=en HTTP/1.1\r\n"
		<< "host: www.localhost.com\r\n"
		<< "\r\n"
		<< "hi it is work";
	cout << bufferStream.str();

	tmp = bufferStream.str();

	Request req = Request::Parse(tmp.c_str());

	cout << req.getVerb() << endl << req.getPath() << endl << req.getProtocolVers() << endl;
	unordered_map<string,string> h = req.getheaders();
	unordered_map<string, string> q = req.getqueries();

	cout << "lang=" << q["lang"].c_str()<< endl;
	cout << "host: " << h["host"].c_str() << endl;
	cout << req.getbody();
}