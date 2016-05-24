#ifndef REGISTRY_FILTER_H
#define REGISTRY_FILTER_H
#pragma once


#include <ntddk.h>

#include "../ResourceList.h"

#define REGISTRY_FILTER_TAG 'FRbO'
#define REGISTRY_FILTER_ALLOCATE(size, pool) ExAllocatePoolWithTag(pool, size, REGISTRY_FILTER_TAG)
#define REGISTRY_FILTER_FREE(ptr) ExFreePoolWithTag(ptr, REGISTRY_FILTER_TAG)

EX_CALLBACK_FUNCTION RegistryFilterCallback;

typedef struct _REGISTRY_FILTER_FILTERED_KEY_ENTRY {
	UNICODE_STRING FullRegistryKeyPath;
	LONG ActionFlags;
	PVOID Reserved;
	RESOURCE_LIST_ENTRY ListEntry;
	WCHAR FullRegistryKeyPathBuffer[1];
} REGISTRY_FILTER_FILTERED_KEY_ENTRY, *PREGISTRY_FILTER_FILTERED_KEY_ENTRY;

typedef struct _REGISTRY_FILTER_CONTEXT {
	PDRIVER_OBJECT DriverObject;
	LARGE_INTEGER FilterContextCookie;
	RESOURCE_LIST_ENTRY_HEAD FilteredRegistryKeysList;
} REGISTRY_FILTER_CONTEXT, *PREGISTRY_FILTER_CONTEXT;

typedef struct _REGISTRY_FILTER_OBJECT_CONTEXT {
	LARGE_INTEGER ObjectContextCookie;
	PREGISTRY_FILTER_FILTERED_KEY_ENTRY RuleEntry;
} REGISTRY_FILTER_OBJECT_CONTEXT, *PREGISTRY_FILTER_OBJECT_CONTEXT;

NTSTATUS RegistryFilterPostCreateKey(
	_In_ PREGISTRY_FILTER_CONTEXT pContext,
	_In_ PREG_POST_CREATE_KEY_INFORMATION Info
);

NTSTATUS RegistryFilterPostCreateKeyEx(
	_In_ PREGISTRY_FILTER_CONTEXT pContext,
	_In_ PREG_POST_OPERATION_INFORMATION Info
);

NTSTATUS RegistryFilterPostOpenKey(
	_In_ PREGISTRY_FILTER_CONTEXT pContext,
	_In_ PREG_POST_OPEN_KEY_INFORMATION Info
);

NTSTATUS RegistryFilterPostOpenKeyEx(
	_In_ PREGISTRY_FILTER_CONTEXT pContext,
	_In_ PREG_POST_OPERATION_INFORMATION Info
);

NTSTATUS RegistryFilterCleanupObjectContext(
	_In_ PREGISTRY_FILTER_CONTEXT pContext,
	_In_ PREG_CALLBACK_CONTEXT_CLEANUP_INFORMATION Info
);

NTSTATUS RegistryFilterPreSetValueKey(
	_In_ PREGISTRY_FILTER_CONTEXT pContext,
	_In_ PREG_SET_VALUE_KEY_INFORMATION Info
);

BOOLEAN IsFilteredRegistryKey(
	_In_      PUNICODE_STRING KeyPath,
	_In_      PREGISTRY_FILTER_CONTEXT Context,
	_Out_opt_ PREGISTRY_FILTER_FILTERED_KEY_ENTRY* EntryOut
);

VOID ReleaseRegistryFilterFilteredKeyEntry(
	_In_ PREGISTRY_FILTER_FILTERED_KEY_ENTRY RuleEntry
);

#endif // !REGISTRY_FILTER_H
