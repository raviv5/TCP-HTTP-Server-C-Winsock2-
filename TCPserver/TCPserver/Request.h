#ifndef  Request_H

#define Request_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <stdexcept>
using namespace std;


class Request
{
public:
	enum eVerb {FORMAT_ERROR = -1 ,HEAD, GET, POST, PUT,DEL, TRACE, OPTIONS }; //supported verbs except -1.
private:
	eVerb m_verb;
	string m_path;
	string m_protocolVers;
	string m_body;
	unordered_map<string, string> m_queries;
	unordered_map<string, string> m_headers;
	int m_indexBuffer = 0; //index for buffer

	//methods
public:
	static Request Parse(const char* i_buffer, int i_length);
	//geters
	eVerb getVerb();
	string getPath();
	string getProtocolVers();
	string getbody();
	unordered_map<string, string> getqueries();
	unordered_map<string, string> getheaders();
private:
	//seterrs
	void setVerb(const char* i_buffer, int i_length);
	void setPath(const char* i_buffer, int i_length);
	void setProtocolVers(const char* i_buffer, int i_length);
	void setbody(const char* i_buffer, int i_length);
	void setQueries(const char* i_buffer, int i_length);
	void setheaders(const char* i_buffer, int i_length);
	void inRange(int i_length);
	
};
#endif // ! Request_H

