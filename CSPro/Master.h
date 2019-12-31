#pragma once
#include <Windows.h>
#include <TlHelp32.h>

class Master {
public:
	Master();
	~Master();
	DWORD getPID(const wchar_t* processName);
	uintptr_t getMAdr(DWORD pID, const wchar_t* moduleName);
	bool openHandle(DWORD pID);
	template<class T>
	T readMem(uintptr_t addr) {
		T result;
		ReadProcessMemory(handle, (LPCVOID)addr, &result, sizeof(result), NULL);
		return result;
	}
	template<class T>
	bool writeMem(uintptr_t addr, T value) {
		if (!WriteProcessMemory(handle, (LPVOID)addr, &value, sizeof(value), NULL)) {
			return false;
		}
		else {
			return true;
		}
	}
	HANDLE handle;
};
	


//class Master
//{
//};
//
