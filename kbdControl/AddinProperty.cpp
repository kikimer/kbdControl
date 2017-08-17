#include "AddinProperty.h"


CAddinProperty::CAddinProperty(const wchar_t* nRus, const wchar_t* nEng, tVariant val, bool isR, bool isW):NameRus(nRus),NameEng(nEng),IsReadable(isR),IsWritable(isW),value(val){
	
	
}

CAddinProperty::~CAddinProperty(void)
{
	clear();
}

void CAddinProperty::clear(void){
	if (TV_VT(&value) == VTYPE_PWSTR && TV_WSTR(&value)!= 0) delete[] TV_WSTR(&value);
}

tVariant CAddinProperty::int2tVar(const int val){
	tVariant res;
	TV_VT(&res) = VTYPE_INT;
	TV_INT(&res) = val;
	TV_WSTR(&res) = NULL;
	return res;
}

tVariant CAddinProperty::wchar2tVar(const wchar_t* val){
	tVariant res;
	DATA_SET_BEGIN(&res);
	int len = std::wcslen(val);//+1 будем считать что 1С ориентируется по wstrLen, а не по '\0'
	TV_VT(&res) = VTYPE_PWSTR;
	TV_WSTR(&res) = new wchar_t[len+1];//но мы будем хранить с '\0'
	res.wstrLen = len;
	memcpy(TV_WSTR(&res), val, (len+1)*sizeof(wchar_t));
	return res;
}

tVariant CAddinProperty::date2tVar(DATE val){
	tVariant res;
	DATA_SET(&res, VTYPE_DATE, date, val);
	return res;
}

tVariant CAddinProperty::bool2tVar(bool val){
	tVariant res;
	DATA_SET(&res, VTYPE_BOOL, bVal, val);
	return res;
}


void CAddinProperty::GetValue(IMemoryManager* alloc, tVariant* retValue)const{
	*retValue = value;
	if(TV_VT(retValue) == VTYPE_PWSTR){
		if (alloc->AllocMemory((void**)&TV_WSTR(retValue), value.wstrLen*sizeof(wchar_t)))
			memcpy(TV_WSTR(retValue), TV_WSTR(&value), value.wstrLen*sizeof(wchar_t));
		else
			TV_VT(retValue) = VTYPE_EMPTY;
	}
}

void CAddinProperty::SetValue(const tVariant* val){
	clear();
	value = *val;
	if (TV_VT(&value) == VTYPE_PWSTR){
		int len = value.wstrLen+1;//мы храним c '\0'
		TV_WSTR(&value) = new WCHAR_T[len+1];
		memcpy(TV_WSTR(&value), TV_WSTR(val), len*sizeof(WCHAR_T));
		TV_WSTR(&value)[len] = 0;
	}

}