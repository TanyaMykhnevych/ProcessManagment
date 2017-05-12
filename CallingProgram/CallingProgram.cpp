#include "stdafx.h"
#include<Windows.h>
#include <tchar.h>
#include <processthreadsapi.h>

#define CrProc(cl, sInfo, procInfo) CreateProcess(0, cl, 0, 0, 0, 0, 0, 0, &sInfo, &procInfo)
#define CrProcWithPar(cl, priority, path, sInfo, procInfo) CreateProcess(0, cl, 0, 0, 0, priority, 0, path, &sInfo, &procInfo)

DWORD  Call()
{
	DWORD returnCode = 1;

	STARTUPINFO sInfo = { sizeof(sInfo) };
	PROCESS_INFORMATION procInfo;
	TCHAR cl[] = _T("NotepadExecutor.exe");

	SYSTEMTIME st;
	GetSystemTime(&st);
	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);

	BOOL bRes = CrProcWithPar(cl, BELOW_NORMAL_PRIORITY_CLASS, 0, sInfo, procInfo);

	if (bRes)
	{
		WaitForSingleObject(procInfo.hProcess, INFINITE);
		GetExitCodeProcess(procInfo.hProcess, &returnCode);

		CloseHandle(procInfo.hThread);
		CloseHandle(procInfo.hProcess);

		returnCode = 0;
	}

	if (returnCode == 0)
	{
		TCHAR cl2[MAX_PATH];
		_stprintf_s(cl2, _T("%s %u %u"), _T("TextFilesHandler.exe"), ft.dwLowDateTime, ft.dwHighDateTime);
		bRes = CrProcWithPar(cl2, 0, _T("../Debug"), sInfo, procInfo);

		if (bRes)
		{
			WaitForSingleObject(procInfo.hProcess, INFINITE);
			GetExitCodeProcess(procInfo.hProcess, &returnCode);

			CloseHandle(procInfo.hThread);
			CloseHandle(procInfo.hProcess);
		}
	}

	return returnCode;
}

int main()
{
	WCHAR returnCode = Call();

	system("pause");

	return returnCode;
}