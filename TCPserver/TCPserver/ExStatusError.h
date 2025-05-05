#ifndef ExStatusError_H
#define ExStatusError_H

#include <stdexcept>
using namespace std;

class ExStatusError : public exception
{
private:
	int m_statusCode;
	string m_statusPhrase;

public:
	
	ExStatusError(int i_code);
	int getStatusCode();
	string what();
};
#endif // !ExStatusError_H

