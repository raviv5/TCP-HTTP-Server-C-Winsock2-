#include "ExStatusError.h"

//-------------------------------------------------------------------------------------------------------------------//
ExStatusError::ExStatusError(int i_code)
{
	m_statusCode = i_code;
	switch (i_code)
	{
	case 400:
	{
		//bad request
		m_statusPhrase = "Bad Request";
		break;
	}
	case 404:
	{
		//Not found
		m_statusPhrase = "Not found";
		break;
	}
	case 405:
	{
		//methode not allow
		m_statusPhrase = "Method Not Allowed.";
		break;
	}
	case 409:
	{
		//Conflict
		m_statusPhrase = "Conflict.";
		break;
	}
	case 500:
	{
		//internel server
		m_statusPhrase = "Internal Server Error";
		break;
	}
	default:
		break;
	}
}
//-------------------------------------------------------------------------------------------------------------------//
string ExStatusError:: what()
{
	return m_statusPhrase;
}
//-------------------------------------------------------------------------------------------------------------------//
int ExStatusError::getStatusCode()
{
	return m_statusCode;
}
//-------------------------------------------------------------------------------------------------------------------//
