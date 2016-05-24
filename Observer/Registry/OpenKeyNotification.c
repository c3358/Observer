#include "Includes.h"

#include "../Log/Log.h"

_Use_decl_annotations_
NTSTATUS RegistryFilterPostOpenKey(
	PREGISTRY_FILTER_CONTEXT pContext,
	PREG_POST_OPEN_KEY_INFORMATION Info
)
{
	NTSTATUS Status;
	PREGISTRY_FILTER_OBJECT_CONTEXT pObjectContext;
	PVOID pOldContext;
	PREGISTRY_FILTER_FILTERED_KEY_ENTRY RuleEntry;

	if (Info->CompleteName == NULL)
	{
		DEBUG_LOG("RegistryFilterPostOpenKey: CompleteName is NULL");
		return STATUS_INVALID_PARAMETER;
	}

	if (!IsFilteredRegistryKey(Info->CompleteName, pContext, &RuleEntry))
	{
		return STATUS_SUCCESS;
	}

	pObjectContext = REGISTRY_FILTER_ALLOCATE(
		sizeof(REGISTRY_FILTER_OBJECT_CONTEXT),
		PagedPool
	);

	if (pObjectContext == NULL)
	{
		DEBUG_LOG("RegistryFilterPostOpenKey: Out of memory");
		ReleaseRegistryFilterFilteredKeyEntry(RuleEntry);
		return STATUS_NO_MEMORY;
	}

	pObjectContext->RuleEntry = RuleEntry;
	pObjectContext->ObjectContextCookie.QuadPart = 0;

	pOldContext = NULL;

	Status = CmSetCallbackObjectContext(
		Info->Object,
		&pObjectContext->ObjectContextCookie,
		(PVOID)pObjectContext,
		&pOldContext
	);

	if (!NT_SUCCESS(Status))
	{
		DEBUG_LOG("RegistryFilterPostOpenKey: CmSetCallbackObjectContext failed with error 0x%.8X", Status);
		REGISTRY_FILTER_FREE(pObjectContext);
		ReleaseRegistryFilterFilteredKeyEntry(RuleEntry);
		return Status;
	}

	return STATUS_SUCCESS;
}

_Use_decl_annotations_
NTSTATUS RegistryFilterPostOpenKeyEx(
	PREGISTRY_FILTER_CONTEXT pContext,
	PREG_POST_OPERATION_INFORMATION Info
)
{
	PCUNICODE_STRING cuRootName;
	UNICODE_STRING FullKeyName;
	PREG_OPEN_KEY_INFORMATION PreInfo;
	PREGISTRY_FILTER_OBJECT_CONTEXT pObjectContext;
	PVOID pOldContext;
	PREGISTRY_FILTER_FILTERED_KEY_ENTRY RuleEntry;
	NTSTATUS Status;
	ULONG TotalUnicodeLength;
	USHORT Count;

	PreInfo = (PREG_OPEN_KEY_INFORMATION)Info->PreInformation;

	if (PreInfo == NULL)
	{
		DEBUG_LOG("RegistryFilterPostOpenKeyEx: PreInformation is NULL");
		return STATUS_INVALID_PARAMETER;
	}

	if (PreInfo->CompleteName == NULL)
	{
		DEBUG_LOG("RegistryFilterPostOpenKeyEx: PreInformation->CompleteName is NULL");
		return STATUS_INVALID_PARAMETER;
	}

	Status = CmCallbackGetKeyObjectID(
		&pContext->FilterContextCookie,
		PreInfo->RootObject,
		NULL,
		&cuRootName
	);

	if (!NT_SUCCESS(Status))
	{
		DEBUG_LOG("RegistryFilterPostOpenKeyEx: CmCallbackGetKeyObjectID failed with error 0x%.8X", Status);
		return Status;
	}

	TotalUnicodeLength = cuRootName->Length + 2 + PreInfo->CompleteName->Length;

	if (TotalUnicodeLength >= 0xFFFF)
	{
		DEBUG_LOG("RegistryFilterPostOpenKeyEx: TotalUnicodeLength >= 0xFFFF");
		return STATUS_NO_MEMORY;
	}

	FullKeyName.Buffer = REGISTRY_FILTER_ALLOCATE(TotalUnicodeLength, NonPagedPool);

	if (FullKeyName.Buffer == NULL)
	{
		DEBUG_LOG("RegistryFilterPostOpenKeyEx: Out of memory");
		return STATUS_NO_MEMORY;
	}

	FullKeyName.Length = (USHORT)TotalUnicodeLength;
	FullKeyName.MaximumLength = (USHORT)TotalUnicodeLength;
	Count = cuRootName->Length >> 1;
	RtlCopyMemory(FullKeyName.Buffer, cuRootName->Buffer, cuRootName->Length);
	FullKeyName.Buffer[Count] = '\\';
	RtlCopyMemory(FullKeyName.Buffer + Count + 1, PreInfo->CompleteName->Buffer, PreInfo->CompleteName->Length);


	if (!IsFilteredRegistryKey(&FullKeyName, pContext, &RuleEntry))
	{
		REGISTRY_FILTER_FREE(FullKeyName.Buffer);
		return STATUS_SUCCESS;
	}
	REGISTRY_FILTER_FREE(FullKeyName.Buffer);

	pObjectContext = REGISTRY_FILTER_ALLOCATE(
		sizeof(REGISTRY_FILTER_OBJECT_CONTEXT),
		PagedPool
	);

	if (pObjectContext == NULL)
	{
		DEBUG_LOG("RegistryFilterPostOpenKeyEx: Out of memory");
		ReleaseRegistryFilterFilteredKeyEntry(RuleEntry);
		return STATUS_NO_MEMORY;
	}

	pObjectContext->RuleEntry = RuleEntry;
	pObjectContext->ObjectContextCookie.QuadPart = 0;

	pOldContext = NULL;

	Status = CmSetCallbackObjectContext(
		Info->Object,
		&pObjectContext->ObjectContextCookie,
		(PVOID)pObjectContext,
		&pOldContext
	);

	if (!NT_SUCCESS(Status))
	{
		DEBUG_LOG("RegistryFilterPostOpenKeyEx: CmSetCallbackObjectContext failed with error 0x%.8X", Status);
		REGISTRY_FILTER_FREE(pObjectContext);
		ReleaseRegistryFilterFilteredKeyEntry(RuleEntry);
		return Status;
	}

	return STATUS_SUCCESS;
}

