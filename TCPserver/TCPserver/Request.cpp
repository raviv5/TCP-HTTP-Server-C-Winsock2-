#include "Request.h"

//-------------------------------------------------------------------------------------------------------------------//
Request Request::Parse(const char* i_buffer, int i_length)
{
	Request request;

	try
	{
		request.setVerb(i_buffer, i_length);
		request.setPath(i_buffer, i_length);
		request.setQueries(i_buffer, i_length);
		request.setProtocolVers(i_buffer, i_length);
		request.setheaders(i_buffer, i_length);
		request.setbody(i_buffer, i_length);
	}
	catch(out_of_range& ex)
	{
		request.m_verb = FORMAT_ERROR;
		request.m_protocolVers = "HTTP/1.1";
	}

	return request;
}
//-------------------------------------------------------------------------------------------------------------------//

Request::eVerb Request::getVerb()
{
	return m_verb;
}
//-------------------------------------------------------------------------------------------------------------------//
string Request::getPath()
{
	return m_path;
}
//-------------------------------------------------------------------------------------------------------------------//
string Request::getProtocolVers()
{
	return m_protocolVers;
}
//-------------------------------------------------------------------------------------------------------------------//
string Request::getbody()
{
	return m_body;
}
//-------------------------------------------------------------------------------------------------------------------//
unordered_map<string, string> Request::getqueries()
{
	return m_queries;
}
//-------------------------------------------------------------------------------------------------------------------//
unordered_map<string, string> Request::getheaders()
{
	return m_headers;
}
//-------------------------------------------------------------------------------------------------------------------//
void Request::setVerb(const char* i_buffer, int i_length)
{
	string verb;
	inRange(i_length);
	while (i_buffer[m_indexBuffer] != ' ')
	{
		verb.push_back(i_buffer[m_indexBuffer++]);
		inRange(i_length);
	}

	const char* cVerb = verb.c_str();
	m_indexBuffer++;
	inRange(i_length);

	if (strcmp(cVerb, "HEAD") == 0)
	{
		m_verb = HEAD;
	}
	else if(strcmp(cVerb, "GET") == 0)
	{
		m_verb = GET;
	}
	else if (strcmp(cVerb, "POST") == 0)
	{
		m_verb = POST;
	}
	else if (strcmp(cVerb, "PUT") == 0)
	{
		m_verb = PUT;
	}
	else if (strcmp(cVerb, "DELETE") == 0)
	{
		m_verb = DEL;
	}
	else if (strcmp(cVerb, "TRACE") == 0)
	{
		m_verb = TRACE;
	}
	else if (strcmp(cVerb, "OPTIONS") == 0)
	{
		m_verb = OPTIONS;
	}
}
//-------------------------------------------------------------------------------------------------------------------//
void Request::setPath(const char* i_buffer, int i_length)
{
	inRange(i_length);
	while (i_buffer[m_indexBuffer] != '?' && i_buffer[m_indexBuffer] != ' ')
	{
		m_path.push_back(i_buffer[m_indexBuffer++]);
		inRange(i_length);
	}
}
//-------------------------------------------------------------------------------------------------------------------//
void Request::setQueries(const char* i_buffer, int i_length)
{
	inRange(i_length);
	if(i_buffer[m_indexBuffer] == '?')
	{
		do
		{
			string param, data;
			m_indexBuffer++;
			inRange(i_length);
			while (i_buffer[m_indexBuffer] != '=')
			{
				param.push_back(i_buffer[m_indexBuffer++]);
				inRange(i_length);
			}
			m_indexBuffer++;
			inRange(i_length);

			while (i_buffer[m_indexBuffer] != '&' && i_buffer[m_indexBuffer] != ' ')
			{
				data.push_back(i_buffer[m_indexBuffer++]);
				inRange(i_length);
			}
			m_queries.insert(pair<string, string>(param, data));
		} 
		while (i_buffer[m_indexBuffer] == '&');
	}

	m_indexBuffer++;
	inRange(i_length);
}
//-------------------------------------------------------------------------------------------------------------------//
void Request::setProtocolVers(const char* i_buffer, int i_length)
{
	inRange(i_length);
	while (i_buffer[m_indexBuffer] != '\r')
	{
		m_protocolVers.push_back(i_buffer[m_indexBuffer++]);
		inRange(i_length);
	}
	m_indexBuffer += 2;
	inRange(i_length);
}
//-------------------------------------------------------------------------------------------------------------------//
void Request::setheaders(const char* i_buffer, int i_length)
{
	inRange(i_length);
	do
	{
		string header, data;

		while (i_buffer[m_indexBuffer] != ':')
		{
			header.push_back(i_buffer[m_indexBuffer++]);
			inRange(i_length);
		}
		m_indexBuffer++;

		if (i_buffer[m_indexBuffer] == ' ')
		{
			m_indexBuffer++;
			inRange(i_length);
		}

		while (i_buffer[m_indexBuffer] != '\r')
		{
			data.push_back(i_buffer[m_indexBuffer++]);
			inRange(i_length);
		}

		m_headers.insert(pair<string, string>(header, data));
		m_indexBuffer += 2;
		inRange(i_length);
	}
	while (i_buffer[m_indexBuffer] != '\r');

	m_indexBuffer +=2;
	inRange(i_length);
}
//-------------------------------------------------------------------------------------------------------------------//
void Request::setbody(const char* i_buffer, int i_length)
{
	inRange(i_length);
	while (i_buffer[m_indexBuffer] != '\0')
	{
		m_body.push_back(i_buffer[m_indexBuffer++]);
		inRange(i_length);
	}

	m_indexBuffer = 0;
}
//-------------------------------------------------------------------------------------------------------------------//
void Request::inRange(int i_length)
{
	if (m_indexBuffer > i_length)
	{
		throw out_of_range("Faulty Http format.");
	}
}
//-------------------------------------------------------------------------------------------------------------------//
