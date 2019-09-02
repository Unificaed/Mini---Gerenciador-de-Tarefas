#include <Windows.h>
#include <Psapi.h>
#include <cstdint>
#include <functional>
#include <Shlwapi.h>
#include <winternl.h>
#include <algorithm>
#include <string>
#include <sddl.h>
#include <iostream>
#include <string>

using namespace std;

#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "ntdll.lib")
#pragma comment(lib, "Shlwapi.lib")

typedef struct _SYSTEM_HANDLE
{
	ULONG ProcessId;
	BYTE ObjectTypeNumber;
	BYTE Flags;
	USHORT Handle;
	PVOID Object;
	ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE, *PSYSTEM_HANDLE;

typedef struct _SYSTEM_HANDLE_INFORMATION
{
	ULONG HandleCount;
	SYSTEM_HANDLE Handles[1];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;