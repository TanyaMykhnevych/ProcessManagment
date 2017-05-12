#include "stdafx.h"
#include <Windows.h>


DWORD RunNotepad()
{
	DWORD returnCode = 1;

	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION procInfo;
	TCHAR cl[] = _T("notepad.exe");

	BOOL bRes = CreateProcess(0, cl, 0, 0, 0, 0, 0, 0, &si, &procInfo);
	if (bRes)
	{
		WaitForSingleObject(procInfo.hProcess, INFINITE);
		GetExitCodeProcess(procInfo.hProcess, &returnCode);
		CloseHandle(procInfo.hThread);
		CloseHandle(procInfo.hProcess);
	}

	return returnCode;
}

int main()
{
	_tprintf(_T("NotepadExecutor has been started!\n"));
	DWORD returnCode = RunNotepad();
	_tprintf(_T("%s Return code: %u!\n\n\n"), !returnCode ? _T("NotepadExecutor has been finished successfully!") : _T("NotepadExecutor has been finished with errors!"), returnCode);
		
	return returnCode;
}

