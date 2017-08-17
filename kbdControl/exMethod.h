#include"KbdControl.h"
#include<sstream>
#include<vector>
#include<set>

void exStart(void* object, const tVariant* param , const long count, tVariant* retValue);

void exStop(void* object, const tVariant* param , const long count, tVariant* retValue);

void exAddKeyCode(void* object, const tVariant* param , const long count, tVariant* retValue);

void exAddAllKeyCode(void* object, const tVariant* param , const long count, tVariant* retValue);

void exClearKeyCode(void* object, const tVariant* param , const long count, tVariant* retValue);

VOID CALLBACK exTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);