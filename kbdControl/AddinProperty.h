#pragma once
#include<string>
#include<types.h>
#include <IMemoryManager.h>

using std::wstring;

class CAddinProperty{
public:
	wstring NameRus;
	wstring NameEng;
	bool IsReadable;
	bool IsWritable;
	
	void GetValue(IMemoryManager* alloc, tVariant* retValue) const;
	void SetValue(const tVariant* val);

	CAddinProperty(const wchar_t* nRus, const wchar_t* nEng, tVariant val, bool isR, bool isW);
	~CAddinProperty(void);

	static tVariant int2tVar(const int);
	static tVariant wchar2tVar(const wchar_t*);
	static tVariant date2tVar(const DATE);
	static tVariant bool2tVar(const bool);
private:
	void clear(void);
	tVariant value;

};

