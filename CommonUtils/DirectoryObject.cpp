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

#define DIRECTORY_QUERY 0x0001
#define DIRECTORY_TRAVERSE 0x0002
#define DIRECTORY_CREATE_OBJECT 0x0004
#define DIRECTORY_CREATE_SUBDIRECTORY 0x0008
#define DIRECTORY_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | 0xF)

typedef NTSTATUS(NTAPI *_NtCreateDirectoryObject)(PHANDLE Handle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes);
typedef VOID(NTAPI *_RtlInitUnicodeString)(PUNICODE_STRING DestinationString, PCWSTR SourceString);

#define DEFINE_NTDLL(x) _ ## x f ## x = (_ ## x)GetNtProcAddress(#x)

LPVOID GetNtProcAddress(LPCSTR name)
{
	return GetProcAddress(GetModuleHandle(L"ntdll"), name);
}

HANDLE CreateObjectDirectory(LPCWSTR dirname)
{
	DEFINE_NTDLL(RtlInitUnicodeString);
	DEFINE_NTDLL(NtCreateDirectoryObject);

	OBJECT_ATTRIBUTES obj_attr;
	UNICODE_STRING obj_name;

	fRtlInitUnicodeString(&obj_name, dirname);

	InitializeObjectAttributes(&obj_attr, &obj_name, OBJ_CASE_INSENSITIVE, nullptr, nullptr);

	HANDLE h = nullptr;

	NTSTATUS status = fNtCreateDirectoryObject(&h, DIRECTORY_ALL_ACCESS, &obj_attr);
	if (status == 0)
	{
		return h;
	}
	else
	{
		SetLastError(NtStatusToDosError(status));
		return nullptr;
	}
}