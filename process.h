#ifndef PROCESS_H
#define PROCESS_H

#include <windows.h>
#include <iostream>
#include <TlHelp32.h>

struct ProcessModule {
	DWORD dwBase;
	DWORD dwSize;
};

class Process {
protected:
	HANDLE hProcess;
	DWORD dwPID;

public:
	Process() {}

	~Process() {
		CloseHandle(hProcess);
	}

	bool Attach(char* processName) {
		HANDLE hPID = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		PROCESSENTRY32 p;
		p.dwSize = sizeof(p);

		do
			if (!strcmp(p.szExeFile, processName))
			{
				dwPID = p.th32ProcessID;
				CloseHandle(hPID);

				hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
				return true;
			}
		while (Process32Next(hPID, &p));

		return false;
	}

	ProcessModule Module(LPSTR moduleName) {
		HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
		MODULEENTRY32 m;
		m.dwSize = sizeof(m);

		do {
			if (!strcmp(m.szModule, moduleName))
			{
				CloseHandle(hModule);

				ProcessModule mod = { (DWORD)m.hModule, m.modBaseSize };
				return mod;
			}
		} while (Module32Next(hModule, &m));

		std::cout << std::endl << "Did not find " << moduleName << "..." << std::endl;
		ProcessModule mod = { (DWORD)false, (DWORD)false };
		return mod;
	}

	bool DataCompare(const BYTE* pData, const BYTE* pMask, const char* pszMask) {
		for (; *pszMask; ++pszMask, ++pData, ++pMask)
			if (*pszMask == 'x' && *pData != *pMask) { return false; }

		return *pszMask == NULL;
	}

	DWORD FindPattern(DWORD start, DWORD size, const char* sig, const char* mask) {
		BYTE* data = new BYTE[size];

		if (!ReadProcessMemory(hProcess, (LPVOID)start, data, size, NULL)) {
			return NULL;
		}

		for (DWORD i = 0; i < size; i++) {
			if (DataCompare((const BYTE*)(data + i), (const BYTE*)sig, mask)) {
				return start + i;
			}
		}

		return NULL;
	}

	DWORD FindPatternArray(DWORD start, DWORD size, const char* mask, int count, ...) {
		char* sig = new char[count + 1];
		va_list ap;
		va_start(ap, count);
		for (int i = 0; i < count; i++) {
			char read = va_arg(ap, char);
			sig[i] = read;
		}
		va_end(ap);
		sig[count] = '\0';

		return FindPattern(start, size, sig, mask);
	}

	template <class T>
	T Read(DWORD dwAddress)
	{
		T read;
		ReadProcessMemory(hProcess, (LPVOID)dwAddress, &read, sizeof(T), NULL);
		return read;
	}

	template <class T>
	void Write(DWORD dwAddress, T Value)
	{
		WriteProcessMemory(hProcess, (LPVOID)dwAddress, &Value, sizeof(T), NULL);
	}

	std::string ReadUnicodeString(DWORD dwAddress, size_t len) {
		std::string str;
		wchar_t wchar;

		for (size_t i = 0; i < len; i++) {
			wchar = Read<wchar_t>(dwAddress + i * sizeof(wchar_t));
			if (wchar > 127/* || wchar == '\0'*/) { continue; }
			str += (char)wchar;
		}

		return str.size() > 0 ? str : "#undef";
	}
};

#endif