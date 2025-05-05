#ifndef Response_H

#define Response_H
#include <iostream>
#include <unordered_map>
#include <string>
#include <ctime>
#include <cstdio>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "ExStatusError.h"
#include "Request.h"
using namespace std;


class Response
{
private:
	string m_protocolVers;
	string m_statusCode;
	string m_statusPhrase;
	string m_body;
	unordered_map<string, string> m_headers;

public:

	static Response To(Request& i_Request);
	string ToString();
	//getters
	string getProtocolVers();
	string getStatusCode();
	string getStatusPhrase();
	string getBody();
	string getHeader(string i_header);
	string filePath(Request& i_Request);

private:
	
	//setters and status
	void  setProtocolVers(string i_protocolVers);
	void  setStatusCode(string i_code);
	void  setStatusPhrase(string i_phrase);
	void  setBody(string i_body);
	void  addHeader(string i_header, string i_data);
	void  okStatus();
	void  createdStatus();
	void  noContentStatus();

	//verbs
	void get(Request& i_Request);
	void post(Request& i_Request);
	void head(Request& i_Request);
	void put(Request& i_Request);
	void del(Request& i_Request);
	void trace(Request& i_Request);
	void options(Request& i_Request);

	static string date();
};



#endif // !Response_H

