#include <ComponentBase.h>
#include "KbdControl.h"

static const wchar_t *ClassNames = L"kbdControl";
static AppCapabilities g_capabilities = eAppCapabilitiesInvalid;

const WCHAR_T* GetClassNames(){
	return ClassNames;	
}

long GetClassObject(const WCHAR_T* wsName, IComponentBase** pInterface)
{
    if(!*pInterface)
    {
        *pInterface= new KbdControl();
        return (long)*pInterface;
    }
    return 0;
}

AppCapabilities SetPlatformCapabilities(const AppCapabilities capabilities)
{
    g_capabilities = capabilities;
    return eAppCapabilitiesLast;
}

long DestroyObject(IComponentBase** pIntf)
{
    if(!*pIntf)
        return -1;

    delete *pIntf;
    *pIntf = 0;
    return 0;
}


