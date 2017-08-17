//В свойства проекта, в определение препроцессора следует добавить ADDINCPP_EXPORTS. Иначе не подключается

#pragma once
#include <ComponentBase.h>
#include <IMemoryManager.h>
#include <AddInDefBase.h>
#include "AddinProperty.h"
#include "AddinMethod.h"
#include <vector>
#include <string>
#include "exMethod.h"

using std::wstring;

class KbdControl :
	public IComponentBase
{
public:

	KbdControl(void);
	virtual ~KbdControl(void);
	//IInitDoneBase
	virtual bool ADDIN_API Init(void*);					//Инициализация и получение указателя на объект 1С Предприятия
    virtual bool ADDIN_API setMemManager(void* mem);	//Получение указателя на аллокатор памяти 1С
    virtual long ADDIN_API GetInfo();					//Возврат версии
    virtual void ADDIN_API Done();						//Завершение работы с компонентой

	//ILanguageExtenderBase
    virtual bool ADDIN_API RegisterExtensionAs(WCHAR_T** wsExtensionName);			//Возврат имени компоненты. По нему будет происходить создание объекта в 1С

	virtual long ADDIN_API GetNProps();												//Возврат количества свойств
	virtual long ADDIN_API FindProp(const WCHAR_T* wsPropName);						//Поиск и возврат индекса свойства по имени
    virtual const WCHAR_T* ADDIN_API GetPropName(long lPropNum, long lPropAlias);	//Возврат указателя на память(выделенную аллокатором), где храниться имя свойства по индексу lPropNum. lPropAlias = 0-англ, 1-локальное
	virtual bool ADDIN_API GetPropVal(const long lPropNum, tVariant* pvarPropVal);	//В pvarPropVal записывается значение свойства по индексу lPropNum. Если значение - строка, то память выделяется аллокатором. Возврат успешности операции(true/false)
	virtual bool ADDIN_API SetPropVal(const long lPropNum, tVariant* varPropVal);	//Установка varPropVal в свойство с индексом lPropNum. Возврат признака успешности операции (true/false)
	virtual bool ADDIN_API IsPropReadable(const long lPropNum);						//Возвращает признак возможности чтения свойства с индексом lPropNum
	virtual bool ADDIN_API IsPropWritable(const long lPropNum);						//Возвращает признак возможности записи свойства с индексом lPropNum
	
	virtual long ADDIN_API GetNMethods();											//Возвращает количество методов
	virtual long ADDIN_API FindMethod(const WCHAR_T* wsMethodName);					//Возвращает индекс метода соответствующего wsMethodName. или -1 если не найден.
    virtual const WCHAR_T* ADDIN_API GetMethodName(const long lMethodNum, const long lMethodAlias);	//Возврат указателя на память(выделенную аллокатором), где храниться имя метода по индексу lMethodNum, или NULL - метод не существует. lMethodAlias - 0-англ, 1-локальное имя.
	virtual long ADDIN_API GetNParams(const long lMethodNum);										//Возврат количества параметров у метода с индексом lMethodNum
    virtual bool ADDIN_API GetParamDefValue(const long lMethodNum, const long lParamNum, tVariant *pvarParamDefValue);	//Установка в pvarParamDefValue значения по умолчанию для метода lMethodNum и параметра lParamNum. VTYPE_EMPTY-нет параметра по умолчанию. Если VTYPE_PSTR, VTYPE_PWSTR или VTYPE_BLOB - надо выделять аллокатором память для объекта. Возвращает успешность операции.
    virtual bool ADDIN_API HasRetVal(const long lMethodNum);															//Возврат признака наличия возвращаемого значения
    virtual bool ADDIN_API CallAsProc(const long lMethodNum, tVariant* paParams, const long lSizeArray);							//Вызов метода индекс lMethodNum, с параметрами paParams в количестве lSizeArray. Возврат успешности операции
	virtual bool ADDIN_API CallAsFunc(const long lMethodNum, tVariant* pvarRetValue, tVariant* paParams, const long lSizeArray);	//Вызов метода индекс lMethodNum, с параметрами paParams в количестве lSizeArray с заполнением возвращаемого значения pvarRetValue (если строка или дв.данные - то alloc). Возврат успешности операции

	//LocaleBase
    virtual void ADDIN_API SetLocale(const WCHAR_T* loc);

	//дополнительные ф-ции
	WCHAR_T* wStr2pwChar(const wstring &wstr); //с размещением в m_iMemory
	static void SendExternalEvent(WCHAR_T* source_, WCHAR_T* event_, WCHAR_T* data_);
	static void addError(const WCHAR_T* descr, unsigned short wcode, long scode);
	static IAddInDefBase	*m_iConnect;
private:
	
	IMemoryManager			*m_iMemory;
	std::vector<CAddinProperty>	m_Properties;
	typedef std::vector<CAddinProperty>::size_type prop_size_t;

	std::vector<CAddinMethod>	m_Methods;
	typedef std::vector<CAddinProperty>::size_type meth_size_t;
	//WCHAR_T			*m_wcLocale;
	
	
};



