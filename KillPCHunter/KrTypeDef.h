#pragma  once
#include <ntifs.h>
#include <ntddk.h>
#pragma warning(disable:4189 4100)

typedef enum _KAPC_ENVIRONMENT
{
	OriginalApcEnvironment,
	AttachedApcEnvironment,
	CurrentApcEnvironment,
	InsertApcEnvironment
} KAPC_ENVIRONMENT;

typedef VOID(*PKNORMAL_ROUTINE) (
	IN PVOID NormalContext,
	IN PVOID SystemArgument1,
	IN PVOID SystemArgument2
	);

typedef VOID(*PKKERNEL_ROUTINE) (
	IN struct _KAPC *Apc,
	IN OUT PKNORMAL_ROUTINE *NormalRoutine,
	IN OUT PVOID *NormalContext,
	IN OUT PVOID *SystemArgument1,
	IN OUT PVOID *SystemArgument2
	);

typedef VOID(*PKRUNDOWN_ROUTINE) (
	IN struct _KAPC *Apc
	);

VOID NTAPI KeInitializeApc(__in PKAPC   Apc,
	__in PKTHREAD     Thread,
	__in KAPC_ENVIRONMENT     TargetEnvironment,
	__in PKKERNEL_ROUTINE     KernelRoutine,
	__in_opt PKRUNDOWN_ROUTINE    RundownRoutine,
	__in PKNORMAL_ROUTINE     NormalRoutine,
	__in KPROCESSOR_MODE  Mode,
	__in PVOID    Context
	);


BOOLEAN  NTAPI KeInsertQueueApc(IN PKAPC Apc,
	IN PVOID SystemArgument1,
	IN PVOID SystemArgument2,
	IN KPRIORITY PriorityBoost);



typedef VOID(NTAPI *fpTypePspExitThread)(
	IN NTSTATUS ExitStatus
	);

#define OFFSET(type, f) ((SIZE_T) \
    ((char *)&((type *)0)->f - (char *)(type *)0))