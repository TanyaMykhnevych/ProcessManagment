#include "stdafx.h"
#include "Windows.h"
#include "tlhelp32.h"

void PrintDllList(DWORD const dwProcessId);
void PrintProcessList();

int main()
{
	PrintProcessList();

	system("pause");
	return 0;
}

void PrintProcessList()
{
	
	HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		return;
	}

	PROCESSENTRY32 peProcessEntry;
	peProcessEntry.dwSize = sizeof(PROCESSENTRY32);

	Process32First(hSnapshot, &peProcessEntry);

	do
	{
		_tprintf(_T("------------ %d\t %s\r\n"), peProcessEntry.cntThreads, peProcessEntry.szExeFile);

		PrintDllList(peProcessEntry.th32ProcessID);
	} while (Process32Next(hSnapshot, &peProcessEntry));

	CloseHandle(hSnapshot);
}

void PrintDllList(DWORD const dwProcessId)
{
	HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);

	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		return;
	}

	MODULEENTRY32 meModuleEntry;
	meModuleEntry.dwSize = sizeof(MODULEENTRY32);

	Module32First(hSnapshot, &meModuleEntry);

	_tprintf(_T("------------------ DLL List: \r\n"));

	do
	{
		_tprintf(_T("------------------ %s \r\n"), meModuleEntry.szModule);
	} while (Module32Next(hSnapshot, &meModuleEntry));

	CloseHandle(hSnapshot);
}
