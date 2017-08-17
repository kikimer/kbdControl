#pragma once

#include<string>
#include<types.h>

using std::wstring;
class CAddinMethod
{
public:
	wstring NameRus;
	wstring NameEng;
	int ParamCount;
	bool hasRetVal;

	typedef void (*function_t)(void* object, const tVariant* param , const long count, tVariant* retValue);
	function_t function;

	void Run(void* object, const tVariant* param, const long count, tVariant* retValue);

	CAddinMethod(const wchar_t* NameRus_, const wchar_t* NameEng_, const int ParamCount_, bool hasRetVal_, function_t function_):NameRus(NameRus_),NameEng(NameEng_),ParamCount(ParamCount_),function(function_){};
	~CAddinMethod(void){};
};

