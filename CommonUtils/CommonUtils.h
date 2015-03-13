#pragma once

#include <Windows.h>
#include <string>

typedef void(__stdcall *console_output)(const char*);

void DebugSetOutput(console_output pout);
void DebugPrintf(const char* lpFormat, ...);
HANDLE CreateSymlink(LPCWSTR linkname, LPCWSTR targetname);
HANDLE CreateObjectDirectory(LPCWSTR dirname);
std::wstring GetErrorMessage(DWORD dwError);
std::wstring GetErrorMessage();
BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
bool CreateRegSymlink(LPCWSTR lpSymlink, LPCWSTR lpTarget, bool bVolatile);
bool DeleteRegSymlink(LPCWSTR lpSymlink);
DWORD NtStatusToDosError(NTSTATUS status);