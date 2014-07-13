#include <ntddk.h>
#include <wdm.h>

//TODO .h
extern void myKiSystemCall64ASM(void);

#define MAX_ACTIVE_CPU 16
#define LSTAR_MSR 0xC0000082

typedef VOID(*KISYSTEMCALL64)(); 
KISYSTEMCALL64 origKiSystemCall64[MAX_ACTIVE_CPU];
KISYSTEMCALL64 myPrologKiSystemCall64;

ULONG active_processor_count;

VOID prologKiSystemCall64(){
	DbgPrint("In other words, please be true\n");
}

VOID DisplayAllLSTAR(){
	GROUP_AFFINITY old_affinity, new_affinity;
	UINT16 current_processor;
	UINT64 tmpLSTAR;
	
	for(current_processor=0; current_processor<active_processor_count; current_processor++){
		memset(&new_affinity, 0, sizeof(GROUP_AFFINITY));
		memset(&old_affinity, 0, sizeof(GROUP_AFFINITY));
		new_affinity.Group = 0;
		new_affinity.Mask = 1ULL << (current_processor);
		KeSetSystemGroupAffinityThread(&new_affinity, &old_affinity);
		tmpLSTAR = __readmsr(LSTAR_MSR);
		DbgPrint("CPU %d LSTAR %p\n", current_processor, tmpLSTAR);
		KeRevertToUserGroupAffinityThread(&old_affinity);
	}
}

VOID HookAllLSTAR(){
	GROUP_AFFINITY old_affinity, new_affinity;
	UINT16 current_processor;
	
	for(current_processor=0; current_processor<active_processor_count; current_processor++){
		memset(&new_affinity, 0, sizeof(GROUP_AFFINITY));
		memset(&old_affinity, 0, sizeof(GROUP_AFFINITY));
		new_affinity.Group = 0;
		new_affinity.Mask = 1ULL << (current_processor);
		KeSetSystemGroupAffinityThread(&new_affinity, &old_affinity);
		origKiSystemCall64[current_processor] = (KISYSTEMCALL64)__readmsr(LSTAR_MSR);
		__writemsr(LSTAR_MSR, (UINT64)myKiSystemCall64ASM);
		KeRevertToUserGroupAffinityThread(&old_affinity);
	}
}

VOID UnhookAllLSTAR(){
	GROUP_AFFINITY old_affinity, new_affinity;
	UINT16 current_processor;
	
	for(current_processor=0; current_processor<active_processor_count; current_processor++){
		if(origKiSystemCall64[current_processor]){
			memset(&new_affinity, 0, sizeof(GROUP_AFFINITY));
			memset(&old_affinity, 0, sizeof(GROUP_AFFINITY));
			new_affinity.Group = 0;
			new_affinity.Mask = 1ULL << (current_processor);
			KeSetSystemGroupAffinityThread(&new_affinity, &old_affinity);
			__writemsr(LSTAR_MSR, (UINT64)origKiSystemCall64[current_processor]);
			KeRevertToUserGroupAffinityThread(&old_affinity);
		}
	}
}

VOID DriverUnload(PDRIVER_OBJECT pDriverObject) {
	DbgPrint("\nFill my heart with song\n");
	
	DisplayAllLSTAR();
	UnhookAllLSTAR();
	DisplayAllLSTAR();
	
	DbgPrint("Let me sing forever more\n");  
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING RegistryPath){
	UINT16 current_processor;
	
	myPrologKiSystemCall64 = prologKiSystemCall64;
	
	for(current_processor=0; current_processor<MAX_ACTIVE_CPU; current_processor++){
		origKiSystemCall64[current_processor] = NULL;
	}
	
	DbgPrint("Fly me to the moon...\n");
	active_processor_count = KeQueryActiveProcessorCount( NULL );
	if(active_processor_count == 0
	|| active_processor_count > MAX_ACTIVE_CPU){
		DbgPrint("Too many processors !\n");
		return STATUS_UNSUCCESSFUL;
	}
	
	DisplayAllLSTAR();
	HookAllLSTAR();
	DisplayAllLSTAR();
	
	DbgPrint("And let me play among the LSTAR !\n");
	pDriverObject->DriverUnload = DriverUnload; 
	return STATUS_SUCCESS;
}  