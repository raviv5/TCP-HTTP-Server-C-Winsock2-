#include "Response.h"

//-------------------------------------------------------------------------------------------------------------------//
Response Response::To(Request& i_Request)
{
	Response response;
	//knowen protocol
	response.setProtocolVers(i_Request.getProtocolVers());
	///add mandatory headers, only the one you know their data.
	response.addHeader("Date", date());
	response.addHeader("Server", "myTCPserver");
	response.addHeader("Content-Type","text/html");

	try
	{
		switch (i_Request.getVerb())
		{
		case Request::HEAD:
		{
			response.head(i_Request);
			break;
		}
		case Request::GET:
		{
			response.get(i_Request);
			break;
		}
		case Request::POST:
		{
			response.post(i_Request);
			break;
		}
		case Request::PUT:
		{
			response.put(i_Request);
			break;
		}
		case Request::DEL:
		{
			response.del(i_Request);
			break;
		}
		case Request::TRACE:
		{
			response.trace(i_Request);
			break;
		}
		case Request::OPTIONS:
		{
			response.options(i_Request);
			break;
		}
		default:
			throw ExStatusError(400);
			break;
		}
	}
	catch (ExStatusError& ex)
	{
		response.setStatusCode(to_string(ex.getStatusCode()));
		if (ex.getStatusCode() == 405)
		{
			response.setBody("Resource already exists.");
		}
		else
		{
			response.setBody(ex.what());
		}
		
	}

	response.addHeader("Content-Length", to_string(response.getBody().size()));

	return response;
}
//-------------------------------------------------------------------------------------------------------------------//
string Response::ToString()
{
	string sResponse;

	sResponse.append(m_protocolVers + ' ' + m_statusCode + ' ' + m_statusPhrase + "\r\n");
	for (auto& pair : m_headers)
	{
		sResponse.append(pair.first + ": " + pair.second + "\r\n");
	}
	sResponse.append("\r\n");
	sResponse.append(m_body);

	return sResponse;
}
//-------------------------------------------------------------------------------------------------------------------//
string Response::getProtocolVers()
{
	return m_protocolVers;
}
//-------------------------------------------------------------------------------------------------------------------//
string Response::getStatusCode()
{
	return m_statusCode;
}
//-------------------------------------------------------------------------------------------------------------------//
string Response::getStatusPhrase()
{
	return m_statusPhrase;
}
//-------------------------------------------------------------------------------------------------------------------//
string Response::getHeader(string i_header)
{
	return m_headers[i_header];
}
//-------------------------------------------------------------------------------------------------------------------//
string Response::getBody()
{
	return m_body;
}
//-------------------------------------------------------------------------------------------------------------------//
void  Response::setProtocolVers(string i_protocolVers)
{
	m_protocolVers = i_protocolVers;
}
//-------------------------------------------------------------------------------------------------------------------//
void  Response::setStatusCode(string i_code)
{
	m_statusCode = i_code;
}
//-------------------------------------------------------------------------------------------------------------------//
void  Response::setStatusPhrase(string i_phrase)
{
	m_statusPhrase = i_phrase;
}
//-------------------------------------------------------------------------------------------------------------------//
void  Response::setBody(string i_body)
{
	m_body = i_body;
}
//-------------------------------------------------------------------------------------------------------------------//
void  Response::addHeader(string i_header, string i_data)
{
	m_headers.insert(pair<string, string>(i_header, i_data));
}
//-------------------------------------------------------------------------------------------------------------------//
void Response::get(Request& i_Request) 
{
	ifstream fileStream(filePath(i_Request).c_str());
	stringstream buffer;
	
	if (!fileStream.good())
	{
		throw ExStatusError(404);
	}
	if (!fileStream.is_open())
	{
		throw ExStatusError(500);
	}
	
	buffer << fileStream.rdbuf();

	// Close the file
	fileStream.close();
	if (fileStream.is_open())
	{
		throw ExStatusError(500);
	}

	// Return the contents of the buffer as a string
	setBody(buffer.str());
	okStatus();
}
//-------------------------------------------------------------------------------------------------------------------//
void Response::post(Request& i_Request)
{

	string pathOfFile = filePath(i_Request);
	ifstream file(pathOfFile);

	//check if exist
	if (file.good())
	{
		file.close();

		throw ExStatusError(405);
	}
	
	ofstream postFile(pathOfFile);

	if (!postFile.is_open())
	{
		throw ExStatusError(500);
	}

	cout << i_Request.getbody();
	postFile << i_Request.getbody();

	postFile.close();

	if (postFile.is_open())
	{
		throw ExStatusError(500);
	}

	createdStatus();
	setBody("Resource created successfully.");
}
//-------------------------------------------------------------------------------------------------------------------//
void Response::put(Request& i_Request)
{
	string pathOfFile = filePath(i_Request);
	ofstream putFile(filePath(i_Request));

	if (!putFile.is_open())
	{
		throw ExStatusError(500);
	}

	putFile << i_Request.getbody();

	putFile.close();

	if (putFile.is_open())
	{
		throw ExStatusError(500);
	}

	okStatus();
	setBody("Resource created/update successfully.");
}
//-------------------------------------------------------------------------------------------------------------------//
void Response::del(Request& i_Request)
{
	ifstream fileStream(filePath(i_Request));

	if (!fileStream.good())
	{
		throw ExStatusError(404);
	}

	fileStream.close();

	if (fileStream.is_open())
	{
		throw ExStatusError(500);
	}
	
	if (remove(filePath(i_Request).c_str()) != 0)
	{
		throw ExStatusError(500);
	}

	noContentStatus();
	setBody("Deletion successful.");
}
//-------------------------------------------------------------------------------------------------------------------//
void Response::trace(Request& i_Request)
{
	setBody(i_Request.getbody());
	okStatus();
}
//-------------------------------------------------------------------------------------------------------------------//
void Response::head(Request& i_Request)
{
	ifstream fileStream(filePath(i_Request));

	if (!fileStream.good())
	{
		throw ExStatusError(404);
	}

	okStatus();
}
//-------------------------------------------------------------------------------------------------------------------//
void Response::options(Request& i_Request)
{
	addHeader("Allow", "HEAD,GET,PUT,POST,DELETE,TRACE,OPTIONS.");
	setBody("COMUNICTION : HEAD,GET,PUT,POST,DELETE,TRACE,OPTIONS.");
	okStatus();
}
//-------------------------------------------------------------------------------------------------------------------//
string Response::date()
{
	time_t now = time(0);
	struct tm tm;
	gmtime_s(&tm, &now); // Use gmtime_s instead of gmtime 

	std::ostringstream dateStream;
	dateStream << std::put_time(&tm, "%a, %d %b %Y %H:%M:%S GMT");
	return dateStream.str();
}
//-------------------------------------------------------------------------------------------------------------------//
string Response::filePath(Request& i_Request)
{
	//extract file path
	string path = i_Request.getPath().insert(0,"C:/temp");
	size_t pos = path.find('.');
	string languge = i_Request.getqueries()["lang"];
	
	if (languge == "he" || languge == "fr")
	{
		path.insert(pos, languge);
	}
	if (languge != "he" && languge != "fr" && languge != "en" && languge != "")
	{
		throw ExStatusError(404);
	}

	return path;
}
//-------------------------------------------------------------------------------------------------------------------//
void  Response::okStatus()
{
	setStatusCode("200");
	setStatusPhrase("Ok");
}
//-------------------------------------------------------------------------------------------------------------------//
void  Response::createdStatus()
{
	setStatusCode("201");
	setStatusPhrase("Created");
}
//-------------------------------------------------------------------------------------------------------------------//
void  Response::noContentStatus()
{
	setStatusCode("204");
	setStatusPhrase("No Content");
}
//-------------------------------------------------------------------------------------------------------------------//