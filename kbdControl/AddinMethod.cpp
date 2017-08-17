#include "AddinMethod.h"


void CAddinMethod::Run(void* object, const tVariant* param, const long count, tVariant* retValue)
{
	(*function)(object, param, count, retValue);
}