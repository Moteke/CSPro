#include "Master.h"
#include <iostream>

Master::Master() {
	handle = NULL;
}

Master::~Master() {
	CloseHandle(handle);
}

DWORD Master::getPID(const wchar_t* processName) {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	DWORD processID = 0;

	if (snapshot == INVALID_HANDLE_VALUE) {
		std::cout << "process snapshot failed...";
		return processID;
	}
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(entry);

	if (Process32First(snapshot, &entry)) {
		do {

			if (!_wcsicmp(processName, entry.szExeFile)) {
				processID = entry.th32ProcessID;
				break;
			}

		} while (Process32Next(snapshot, &entry));
	}
	else {
		std::cout << "process32 failed...";
	}

	CloseHandle(snapshot);
	return processID;
}

uintptr_t Master::getMAdr(DWORD pID, const wchar_t* moduleName) {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pID);
	uintptr_t moduleAddress = 0;

	if (snapshot == INVALID_HANDLE_VALUE) {
		std::cout << "module snapshot failed...";
		return 0;
	}

	MODULEENTRY32 entry;
	entry.dwSize = sizeof(entry);

	if (Module32First(snapshot, &entry)) {

		do {

			if (!_wcsicmp(entry.szModule, moduleName)) {
				moduleAddress = (uintptr_t)entry.modBaseAddr;
				break;
			}

		} while (Module32Next(snapshot, &entry));

	}
	else {
		std::cout<< GetLastError();
		return 0;
	}
	
	CloseHandle(snapshot);
	return moduleAddress;
}

bool Master::openHandle(DWORD pID) {
	handle = OpenProcess(PROCESS_ALL_ACCESS, 0, pID);
	if (!handle) {
		return false;
	}
	else {
		return true;
	}
}