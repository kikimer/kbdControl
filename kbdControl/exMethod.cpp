#include "exMethod.h"

UINT_PTR timer_id; //
HHOOK hhook = NULL;
HINSTANCE hDLL;
std::vector<wchar_t> ScanCode;
std::set<int> KeyCodes;

KbdControl *kbdObject = NULL;
bool AllKeyCodes;
bool ScanMode=false;



BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved){
	hDLL = hModule; 
	
	return true;
}

void exStart(void* object, const tVariant* param , const long count, tVariant* retValue){
	kbdObject = (KbdControl*) object;
	

	ScanMode = false;
	ScanCode.clear();
	if (hhook != NULL) UnhookWindowsHookEx(hhook);
	hhook = SetWindowsHookEx(WH_KEYBOARD, &KeyboardProc, hDLL, GetCurrentThreadId());

}

void exStop(void* object, const tVariant* param , const long count, tVariant* retValue){
	if (timer_id != 0) {
		KillTimer(NULL, timer_id);
		timer_id = 0;
	}
	if (hhook != NULL) {
		UnhookWindowsHookEx(hhook);
		hhook = NULL;
	}


}

void exAddKeyCode(void* object, const tVariant* param , const long count, tVariant* retValue){
	KeyCodes.insert(TV_INT(param));
}

void exAddAllKeyCode(void* object, const tVariant* param , const long count, tVariant* retValue){
	AllKeyCodes = true;
}

void exClearKeyCode(void* object, const tVariant* param , const long count, tVariant* retValue){
	KeyCodes.clear();
	AllKeyCodes = false;
}

VOID CALLBACK exTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime){
	if(timer_id == 0) return;
	KillTimer(NULL, timer_id);
	timer_id = 0;
	ScanMode = false;

	std::wstringstream wss;
	std::wstring wstr;
	for(std::vector<wchar_t>::const_iterator i = ScanCode.begin(); i != ScanCode.end(); ++i)
		wss << *i;
	ScanCode.clear();
	wss>>wstr;
	size_t len = wcslen(wstr.c_str())+1;
	if(len != 1){
		wchar_t* keyCode = new wchar_t[len];
		memcpy(keyCode, wstr.c_str(),len*sizeof(wchar_t));
		KbdControl::SendExternalEvent(L"KbdControl", L"Scan", keyCode);
		delete[] keyCode;
	}
}

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam){
	//code - определение как использовать вызов
	//wParam - виртуальный код
	//lParam - битовой набор даных о нажатой клавише

	bool keyPress = (lParam & 0x80000000) == 0; 
	if(!keyPress || code != HC_ACTION) return CallNextHookEx(0, code, wParam, lParam);

	tVariant temp;
	kbdObject->GetPropVal(kbdObject->FindProp(L"Prefix"), &temp);
	int prefix = (TV_VT(&temp) == VTYPE_I4)? TV_I4(&temp): 0;
	
	kbdObject->GetPropVal(kbdObject->FindProp(L"Suffix"), &temp);
	int suffix = (TV_VT(&temp) == VTYPE_I4)? TV_I4(&temp): 0;

	kbdObject->GetPropVal(kbdObject->FindProp(L"WaitSuffix"), &temp);
	int waitsuffix = (TV_VT(&temp) == VTYPE_I4)? TV_I4(&temp): 0;

	bool scanActive = prefix != 0 && (suffix !=0 || waitsuffix != 0);
	
	if (scanActive) {
		if (wParam == prefix && !ScanMode){
			ScanMode = true;
			if(waitsuffix>0) timer_id=SetTimer(NULL, 0, (UINT)waitsuffix*1000,&exTimerProc);
			return 1;
		}
		if (wParam == suffix && ScanMode){
			ScanMode = false;
			if(timer_id != 0) {
				KillTimer(NULL, timer_id);
				timer_id = 0;
			}
			std::wstringstream wss;
			std::wstring wstr;
			for(std::vector<wchar_t>::const_iterator i = ScanCode.begin(); i != ScanCode.end(); ++i)
				wss << *i;
			ScanCode.clear();
			wss>>wstr;
		
			size_t len = wcslen(wstr.c_str())+1;
			if(len != 1){
				wchar_t* keyCode = new wchar_t[len];
				memcpy(keyCode, wstr.c_str(),len*sizeof(wchar_t));
				KbdControl::SendExternalEvent(L"KbdControl", L"Scan", keyCode);
				delete[] keyCode;
			}
		
			return 1;
		}
		if(ScanMode){

			BYTE keyState[256];
			GetKeyboardState(keyState);
			wchar_t buff;
			ToUnicode(wParam,lParam,keyState,&buff,1,0);
			ScanCode.push_back(buff);
			return 1;
		}
	}	
	if(AllKeyCodes || KeyCodes.find(wParam)!=KeyCodes.end())
	{
		std::wstringstream wss;
		std::wstring wstr;
		wss << wParam;
		wss >> wstr;
		size_t len = wcslen(wstr.c_str())+1;
		wchar_t* keyCode = new wchar_t[len];
		memcpy(keyCode, wstr.c_str(),len*sizeof(wchar_t));
	
		KbdControl::SendExternalEvent(L"KbdControl", L"KeyPress", keyCode);
		delete[] keyCode;
		return 1;
	}
	return 0; // >0 : прерывает выполнение последующих подписок
}




