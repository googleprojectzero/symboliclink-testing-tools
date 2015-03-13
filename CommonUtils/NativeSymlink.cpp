//  Copyright 2015 Google Inc. All Rights Reserved.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//  http ://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#include "stdafx.h"
#include <winternl.h>
#include "CommonUtils.h"

#define SYMBOLIC_LINK_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | 0x1)

typedef NTSTATUS(NTAPI* fNtCreateSymbolicLinkObject)(PHANDLE LinkHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PUNICODE_STRING TargetName);
typedef VOID(NTAPI *fRtlInitUnicodeString)(PUNICODE_STRING DestinationString, PCWSTR SourceString);

FARPROC GetProcAddressNT(LPCSTR lpName)
{
	return GetProcAddress(GetModuleHandleW(L"ntdll"), lpName);
}

HANDLE CreateSymlink(LPCWSTR linkname, LPCWSTR targetname)
{
	fRtlInitUnicodeString pfRtlInitUnicodeString = (fRtlInitUnicodeString)GetProcAddressNT("RtlInitUnicodeString");
	fNtCreateSymbolicLinkObject pfNtCreateSymbolicLinkObject = (fNtCreateSymbolicLinkObject)GetProcAddressNT("NtCreateSymbolicLinkObject");

	OBJECT_ATTRIBUTES objAttr;
	UNICODE_STRING name;
	UNICODE_STRING target;

	pfRtlInitUnicodeString(&name, linkname);
	pfRtlInitUnicodeString(&target, targetname);

	InitializeObjectAttributes(&objAttr, &name, OBJ_CASE_INSENSITIVE, nullptr, nullptr);	

	HANDLE hLink;

	NTSTATUS status = pfNtCreateSymbolicLinkObject(&hLink, SYMBOLIC_LINK_ALL_ACCESS, &objAttr, &target);
	if (status == 0)
	{
		DebugPrintf("Opened Link %ls -> %ls: %p\n", linkname, targetname, hLink);
		return hLink;
	}
	else
	{
		SetLastError(NtStatusToDosError(status));
		return nullptr;
	}
}

