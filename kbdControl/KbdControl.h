//� �������� �������, � ����������� ������������� ������� �������� ADDINCPP_EXPORTS. ����� �� ������������

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
	virtual bool ADDIN_API Init(void*);					//������������� � ��������� ��������� �� ������ 1� �����������
    virtual bool ADDIN_API setMemManager(void* mem);	//��������� ��������� �� ��������� ������ 1�
    virtual long ADDIN_API GetInfo();					//������� ������
    virtual void ADDIN_API Done();						//���������� ������ � �����������

	//ILanguageExtenderBase
    virtual bool ADDIN_API RegisterExtensionAs(WCHAR_T** wsExtensionName);			//������� ����� ����������. �� ���� ����� ����������� �������� ������� � 1�

	virtual long ADDIN_API GetNProps();												//������� ���������� �������
	virtual long ADDIN_API FindProp(const WCHAR_T* wsPropName);						//����� � ������� ������� �������� �� �����
    virtual const WCHAR_T* ADDIN_API GetPropName(long lPropNum, long lPropAlias);	//������� ��������� �� ������(���������� �����������), ��� ��������� ��� �������� �� ������� lPropNum. lPropAlias = 0-����, 1-���������
	virtual bool ADDIN_API GetPropVal(const long lPropNum, tVariant* pvarPropVal);	//� pvarPropVal ������������ �������� �������� �� ������� lPropNum. ���� �������� - ������, �� ������ ���������� �����������. ������� ���������� ��������(true/false)
	virtual bool ADDIN_API SetPropVal(const long lPropNum, tVariant* varPropVal);	//��������� varPropVal � �������� � �������� lPropNum. ������� �������� ���������� �������� (true/false)
	virtual bool ADDIN_API IsPropReadable(const long lPropNum);						//���������� ������� ����������� ������ �������� � �������� lPropNum
	virtual bool ADDIN_API IsPropWritable(const long lPropNum);						//���������� ������� ����������� ������ �������� � �������� lPropNum
	
	virtual long ADDIN_API GetNMethods();											//���������� ���������� �������
	virtual long ADDIN_API FindMethod(const WCHAR_T* wsMethodName);					//���������� ������ ������ ���������������� wsMethodName. ��� -1 ���� �� ������.
    virtual const WCHAR_T* ADDIN_API GetMethodName(const long lMethodNum, const long lMethodAlias);	//������� ��������� �� ������(���������� �����������), ��� ��������� ��� ������ �� ������� lMethodNum, ��� NULL - ����� �� ����������. lMethodAlias - 0-����, 1-��������� ���.
	virtual long ADDIN_API GetNParams(const long lMethodNum);										//������� ���������� ���������� � ������ � �������� lMethodNum
    virtual bool ADDIN_API GetParamDefValue(const long lMethodNum, const long lParamNum, tVariant *pvarParamDefValue);	//��������� � pvarParamDefValue �������� �� ��������� ��� ������ lMethodNum � ��������� lParamNum. VTYPE_EMPTY-��� ��������� �� ���������. ���� VTYPE_PSTR, VTYPE_PWSTR ��� VTYPE_BLOB - ���� �������� ����������� ������ ��� �������. ���������� ���������� ��������.
    virtual bool ADDIN_API HasRetVal(const long lMethodNum);															//������� �������� ������� ������������� ��������
    virtual bool ADDIN_API CallAsProc(const long lMethodNum, tVariant* paParams, const long lSizeArray);							//����� ������ ������ lMethodNum, � ����������� paParams � ���������� lSizeArray. ������� ���������� ��������
	virtual bool ADDIN_API CallAsFunc(const long lMethodNum, tVariant* pvarRetValue, tVariant* paParams, const long lSizeArray);	//����� ������ ������ lMethodNum, � ����������� paParams � ���������� lSizeArray � ����������� ������������� �������� pvarRetValue (���� ������ ��� ��.������ - �� alloc). ������� ���������� ��������

	//LocaleBase
    virtual void ADDIN_API SetLocale(const WCHAR_T* loc);

	//�������������� �-���
	WCHAR_T* wStr2pwChar(const wstring &wstr); //� ����������� � m_iMemory
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



