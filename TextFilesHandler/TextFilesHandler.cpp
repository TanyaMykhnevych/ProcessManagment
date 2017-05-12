#include "stdafx.h"
#include <Windows.h>
#include <clocale>
#include <fstream>


bool isUnicodeFile(FILE* fileptr, const TCHAR* filename, BYTE **resBuf)
{
	BYTE *buffer;
	long filelen;
	int flag = -1;
	bool res = false;

	_tfopen_s(&fileptr, filename, TEXT("rb"));  //rb - binary mode
	fseek(fileptr, 0, SEEK_END);          // to the end of the file
	filelen = ftell(fileptr);             // bytes count from the begginig
	rewind(fileptr);                      // back to the beginning

	buffer = (BYTE *)malloc((filelen + 2) * sizeof(BYTE)); // Memory for file + \0
	fread((void*)buffer, filelen, 1, fileptr);
	fclose(fileptr); // Close the file
	buffer[filelen] = buffer[filelen + 1] = '\0';

	if (IsTextUnicode(buffer, filelen, &flag))
	{
		res = true;
	}
	else
	{
		res = false;
	}
	*resBuf = buffer;

	return res;
}

int PrintFileLinesLength(const TCHAR* fileName)
{
	FILE* fp;
	BYTE *buf;
	DWORD lineSymbolsCount = 0;
	DWORD lines = 1;

	_wfopen_s(&fp, fileName, _T("r"));

	BOOL isUnicode = isUnicodeFile(fp, fileName, &buf);

	if (fp == nullptr)
		return -1;

	fclose(fp);

	if (isUnicode)
	{
		wchar_t *ubuf = (wchar_t*)buf;
		for (size_t i = 1; ubuf[i] != '\0'; i++)
		{
			if (ubuf[i] == _T('\n'))
			{
				_tprintf(_T("\tLine %d: %d\n"), lines, lineSymbolsCount);

				lineSymbolsCount = 0;
				lines++;
			}
			else if (ubuf[i] != _T('\r')) {
				lineSymbolsCount++;
			}
		}
	}
	else
	{
		char *abuf = (char*)buf;
		for (size_t i = 0; abuf[i] != '\0'; i++)
		{
			if (abuf[i] == _T('\n'))
			{
				_tprintf(_T("\tLine %d: %d\n"), lines, lineSymbolsCount);

				lineSymbolsCount = 0;
				lines++;
			}
			else if (abuf[i] != _T('\r')) {
				lineSymbolsCount++;
			}
		}
	}
	_tprintf(_T("\tLine %d: %d\n"), lines, lineSymbolsCount);

	return lines;
}

DWORD CheckTextFiles(_TCHAR* arguments[], int argumentsCount)
{
	DWORD returnCode = 1;

	if (argumentsCount == 3)
	{
		int counter = 0;
		WIN32_FIND_DATA fData;
		TCHAR* pattern = _T("*.txt");
		HANDLE hFile = FindFirstFile(pattern, &fData);

		if (hFile != INVALID_HANDLE_VALUE)
		{
			FILETIME fTime = {};
			fTime.dwLowDateTime = wcstod(arguments[1], 0);
			fTime.dwHighDateTime = wcstod(arguments[2], 0);

			_tprintf(_T("--------------------Files----------------------\n"));

			do
			{
				if (CompareFileTime(&fTime, &fData.ftLastWriteTime) < 0)
				{
					_tprintf(_T("\n\n¹ %d\nName: %s\n"), ++counter, fData.cFileName);					

					DWORD c = PrintFileLinesLength(fData.cFileName);

					_tprintf(_T("\nLines count: %d\n"), c);
				}
			} while (FindNextFile(hFile, &fData));
			FindClose(hFile);
		}

		// means no created files found
		if (counter == 0)
		{
			_tprintf(_T("%s\n"), _T("There are no created files."));
		}

		returnCode = 0;
	}

	return returnCode;
}


int _tmain(int argc, _TCHAR* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	_tsetlocale(LC_ALL, _T("Russian"));

	_tprintf(_T("NotepadExecutor has been started!\n\n"));
	DWORD returnCode = CheckTextFiles(argv, argc);
	_tprintf(_T("\n\n%s Return code: %u!\n\n"), !returnCode ? _T("TextFilesHandler has been finished successfully!") : _T("TextFilesHandler has been finished with errors!"), returnCode);

	return returnCode;
}


