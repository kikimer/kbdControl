#include "KbdControl.h"

IAddInDefBase *KbdControl::m_iConnect = NULL;

WCHAR_T* KbdControl::wStr2pwChar(const wstring &wstr)
{
		int len = wstr.length();
		WCHAR_T *result;
		m_iMemory->AllocMemory((void**)&result, (len+1)*sizeof(WCHAR_T));
		for(int i=0; i<len; i++){
			result[i] = wstr[i];
		}
		result[len] = 0;
		return result;
	}

KbdControl::KbdControl(){
	m_Properties.push_back(CAddinProperty(L"Префикс",L"Prefix",CAddinProperty::int2tVar(0),true, true));
	m_Properties.push_back(CAddinProperty(L"Суфикс",L"Suffix",CAddinProperty::int2tVar(0),true, true));
	m_Properties.push_back(CAddinProperty(L"ОжиданиеСуфикса",L"WaitSuffix",CAddinProperty::int2tVar(10),true, true));
	
	m_Methods.push_back(CAddinMethod(L"Запустить", L"Start",0,false, &exStart));
	m_Methods.push_back(CAddinMethod(L"Остановить", L"Stop",0,false, &exStop));
	m_Methods.push_back(CAddinMethod(L"ДобавитьКодКлавиши", L"AddKeyCode",1,false, &exAddKeyCode));
	m_Methods.push_back(CAddinMethod(L"ДобавитьВсеКодыКлавиш", L"AddAllKeyCode",0,false, &exAddAllKeyCode));
	m_Methods.push_back(CAddinMethod(L"ОчиститьКодыКлавиш", L"ClearKeyCode",0,false, &exClearKeyCode));
}

KbdControl::~KbdControl(){
}

//IInitDoneBase
//---------------------------------------------------------------------------//
bool KbdControl::Init(void* pConnection){
	m_iConnect = (IAddInDefBase*)pConnection;
	return m_iConnect != NULL;
}

bool KbdControl::setMemManager(void* mem){
	m_iMemory = (IMemoryManager*) mem;
	return m_iMemory != NULL;
}

long KbdControl::GetInfo()
{ 
   return 2000; 
}

void KbdControl::Done(){
	m_iMemory = NULL;
	m_iConnect = NULL;
}


// ILanguageExtenderBase
//---------------------------------------------------------------------------//
bool KbdControl::RegisterExtensionAs(WCHAR_T** wsExtensionName)
{ 
    const wchar_t *wcExName = L"kbdControl";
    int iExSize = wcslen(wcExName) + 1;

    if (m_iMemory)
    {
        if(m_iMemory->AllocMemory((void**)wsExtensionName, iExSize * sizeof(WCHAR_T)))
            memset(*wsExtensionName, 0, iExSize * sizeof(WCHAR_T));
		    memcpy(*wsExtensionName, wcExName, iExSize * sizeof(WCHAR_T));
        return true;
    }

    return false; 
}

long KbdControl::GetNProps()
{ 
    return m_Properties.size();
}
long KbdControl::FindProp(const WCHAR_T* wsPropName)
{
	for (size_t i = 0; i < m_Properties.size(); i++){
		if (m_Properties[i].NameRus == wsPropName) return i;
		if (m_Properties[i].NameEng == wsPropName) return i;
	}
	return -1L;
}
 const WCHAR_T* KbdControl::GetPropName(long lPropNum, long lPropAlias){
	 if (lPropNum < 0) return NULL;
	 if ((prop_size_t)lPropNum >= m_Properties.size()) return NULL;
	 if(lPropAlias==0) 
		 return wStr2pwChar(m_Properties[lPropNum].NameEng);
	 else
		 return wStr2pwChar(m_Properties[lPropNum].NameRus);	 
}
bool KbdControl::GetPropVal(const long lPropNum, tVariant* pvarPropVal){
	if (lPropNum < 0) return false;
	if ((prop_size_t)lPropNum >= m_Properties.size()) return false;
	m_Properties[lPropNum].GetValue(m_iMemory, pvarPropVal);
	return true;
}
bool KbdControl::SetPropVal(const long lPropNum, tVariant* varPropVal){
	if (lPropNum < 0) return false;
	if ((prop_size_t)lPropNum >= m_Properties.size()) return false;
	m_Properties[lPropNum].SetValue(varPropVal);
	return true;
}
bool KbdControl::IsPropReadable(const long lPropNum){
	if (lPropNum < 0) return false;
	if ((prop_size_t)lPropNum >= m_Properties.size()) return false;
	return m_Properties[lPropNum].IsReadable;
}
bool KbdControl::IsPropWritable(const long lPropNum){
	if (lPropNum < 0) return false;

	if ((prop_size_t)lPropNum >= m_Properties.size()) return false;
	return m_Properties[lPropNum].IsWritable;
}

long KbdControl::GetNMethods(){
	return m_Methods.size();
}

long KbdControl::FindMethod(const WCHAR_T* wsMethodName){
	for (size_t i = 0; i < m_Methods.size(); i++){
		if (m_Methods[i].NameRus == wsMethodName) return i;
		if (m_Methods[i].NameEng == wsMethodName) return i;
	}

	return -1L;
}

const WCHAR_T* KbdControl::GetMethodName(const long lMethodNum, const long lMethodAlias){
	 if (lMethodNum < 0) return NULL;
	 if ((meth_size_t)lMethodNum >= m_Methods.size()) return NULL;
	 if(lMethodAlias==0) 
		 return wStr2pwChar(m_Methods[lMethodNum].NameEng);
	 else
		 return wStr2pwChar(m_Methods[lMethodNum].NameRus);
}

long KbdControl::GetNParams(const long lMethodNum){
	if (lMethodNum < 0) return 0;
	if ((meth_size_t)lMethodNum >= m_Methods.size()) return 0;
	return m_Methods[lMethodNum].ParamCount;
}

bool KbdControl::GetParamDefValue(const long lMethodNum, const long lParamNum, tVariant *pvarParamDefValue){
	//Без реализации параметров по умолчанию
	TV_VT(pvarParamDefValue) = VTYPE_EMPTY;
	return true;
}

bool KbdControl::HasRetVal(const long lMethodNum){
	if (lMethodNum < 0) return false;
	if ((meth_size_t)lMethodNum >= m_Methods.size()) return false;
	return m_Methods[lMethodNum].hasRetVal;
}

bool KbdControl::CallAsProc(const long lMethodNum, tVariant* paParams, const long lSizeArray){
	if (lMethodNum < 0) return false;
	if ((meth_size_t)lMethodNum >= m_Methods.size()) return false;
	m_Methods[lMethodNum].Run(this, paParams, lSizeArray, NULL);
	return true;
}

bool KbdControl::CallAsFunc(const long lMethodNum, tVariant* pvarRetValue, tVariant* paParams, const long lSizeArray){
	if (lMethodNum < 0) return false;
	if ((meth_size_t)lMethodNum >= m_Methods.size()) return false;
	m_Methods[lMethodNum].Run(this, paParams, lSizeArray, pvarRetValue);
	return true;
}	

void KbdControl::SetLocale(const WCHAR_T* loc){
	
}

void KbdControl::SendExternalEvent(WCHAR_T* source_, WCHAR_T* event_, WCHAR_T* data_){
	m_iConnect->ExternalEvent(source_, event_, data_);	//мы и выделяем и уничтожаем 
}

void KbdControl::addError(const WCHAR_T* descr, unsigned short wcode, long scode){
	m_iConnect->AddError(wcode, L"kdbControl", descr, scode);
}