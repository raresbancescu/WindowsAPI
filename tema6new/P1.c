#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <tchar.h>
#include <winnt.h>
#include <processthreadsapi.h>

void write_in_file(const char *path, char *text) {

    HANDLE handle;
    BOOL ErrorFlag;
    DWORD NumberOfLetters = (DWORD) strlen(text);
    DWORD NumberOfLettersWritten = 0;
    handle = CreateFileA(path, FILE_APPEND_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE) {
        // printf("Eroare la deschiderea fisierului");
        CloseHandle(handle);
    }

    ErrorFlag = WriteFile(handle, text, NumberOfLetters, &NumberOfLettersWritten, NULL);

    if (ErrorFlag == 0) {

        printf("Nu s e putut scrie in fisier");
        CloseHandle(handle);
    } else {
        if (NumberOfLetters != NumberOfLettersWritten) {
            printf("Nu s au putut scrie toate literele");
            CloseHandle(handle);
        } else {
            // printf("S-au scris toate literele");
            CloseHandle(handle);
        }
    }
}


void GetComputerInfoHT() {
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION info = NULL;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = NULL;
    BOOL ok = 0;
    DWORD length = 0;
    BOOL success;
    DWORD numaNodes = 0;
    DWORD processorCores = 0;
    DWORD cachel1 = 0;
    DWORD cachesizel1 = 0;
    DWORD cachel2 = 0;
    DWORD cachesizel2 = 0;
    DWORD cachel3 = 0;
    DWORD cachesizel3 = 0;
    DWORD processorPackage = 0;
    while (ok == 0) {
        success = GetLogicalProcessorInformation(info, &length);
        if (!success) {
            //printf("Eroare \n");
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
                if (info) {
                    free(info);
                }
                info = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION) malloc(length);
                if (info == NULL) {
                    printf("Eroare la alocarea memoriei\n");
                }
            } else {
                printf("Alta eroare %d\n", GetLastError());
            }
        } else {
            ok = 1;
            //printf("A functionat\n");
        }
    }
    ptr = info;
    DWORD currentPointer = 0;
    while (currentPointer <= length) {
        switch (ptr->Relationship) {
            case RelationNumaNode: {
                numaNodes++;
                break;
            }
            case RelationProcessorCore: {
                processorCores++;
                break;
                //logical processorcount
            }
            case RelationCache: {
                PCACHE_DESCRIPTOR c = &ptr->Cache;
                if (c->Level == 1) {
                    cachel1++;
                    cachesizel1 += c->Size;
                } else if (c->Level == 2) {
                    cachel2++;
                    cachesizel2 += c->Size;
                } else if (c->Level == 3) {
                    cachel3++;
                    cachesizel3 += c->Size;
                }
                break;
            }
            case RelationProcessorPackage: {
                processorPackage++;
                break;
            }
        }
        currentPointer += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
        ptr++;
    }
    char buffer[100];
    const char *path = "C:\\Facultate\\CSSO\\Laboratoare\\Week6\\info.txt";
    sprintf(buffer, "a)\n");
    write_in_file(path, buffer);
    sprintf(buffer, "Numa nodes:%d\n", numaNodes);
    write_in_file(path, buffer);
    printf("Numa nodes:%d\n", numaNodes);
    sprintf(buffer, "Processor cores: %d\n", processorCores);
    write_in_file(path, buffer);
    printf("Processor cores: %d\n", processorCores);
    sprintf(buffer, "Cache l1 number: %d size: %d\n", cachel1, cachesizel1);
    write_in_file(path, buffer);
    printf("Cache l1 number: %d size: %d\n", cachel1, cachesizel1);
    sprintf(buffer, "Cache l2 number: %d size: %d\n", cachel2, cachesizel2);
    write_in_file(path, buffer);
    printf("Cache l2 number: %d size: %d\n", cachel2, cachesizel2);
    sprintf(buffer, "Cache l3 number: %d size: %d\n", cachel3, cachesizel3);
    write_in_file(path, buffer);
    printf("Cache l3 number: %d size: %d\n", cachel3, cachesizel3);
    sprintf(buffer, "ProcessorPackage: %d\n", processorPackage);
    write_in_file(path, buffer);
    printf("ProcessorPackage: %d\n", processorPackage);


    for (int i = 0; i < length / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); i++) {
        if (info[i].Relationship == RelationProcessorCore && info[i].ProcessorCore.Flags) {
            sprintf(buffer, "HT logical processor %d\n", info[i].ProcessorMask);
            write_in_file(path, buffer);
            printf("HT logical processor %d\n", info[i].ProcessorMask);
        } else {
            //printf("Not HT logical processor %d\n",info[i].ProcessorMask);
        }
    }

}

void getComputerInfoNUMA() {
    const char *path = "C:\\Facultate\\CSSO\\Laboratoare\\Week6\\info.txt";
    char buffer[100];
    sprintf(buffer, "b)\n");
    write_in_file(path, buffer);
    DWORD currentProcessId = GetCurrentProcessId();
    HANDLE handleOfProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, currentProcessId);
    DWORD_PTR processAffinityMask = 0;
    DWORD_PTR systemAffinityMask = 0;
    if (handleOfProcess == NULL) {
        printf("Eroare la obtinere handle proces");
    }
    if (!GetProcessAffinityMask(handleOfProcess, &processAffinityMask, &systemAffinityMask)) {
        printf("Eroare la gerProcessAffinityMask");
    } else {
        sprintf(buffer, "lpProcessAffinityMask: %ld\n", processAffinityMask);
        write_in_file(path, buffer);
        printf("lpProcessAffinityMask: %ld\n", processAffinityMask);
        sprintf(buffer, "lpSystemAffinityMask: %ld\n", systemAffinityMask);
        write_in_file(path, buffer);
        printf("lpSystemAffinityMask: %ld\n", systemAffinityMask);
    }
    // total number of nodes:
    DWORD highestNodeNumber = 0;
    GetNumaHighestNodeNumber(&highestNodeNumber);
    sprintf(buffer, "Total number of nodes: %ld\n", highestNodeNumber + 1);
    write_in_file(path, buffer);
    printf("Total number of nodes: %ld\n", highestNodeNumber + 1);
}

void getComputerInfoCPUSets() {
    const char *path = "C:\\Facultate\\CSSO\\Laboratoare\\Week6\\info.txt";
    char buffer[100];
    sprintf(buffer, "c)\n");
    write_in_file(path, buffer);
    DWORD currentProcessId = GetCurrentProcessId();
    HANDLE currentProcessHandle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, currentProcessId);
    if (currentProcessHandle == NULL) {
        printf("Eroare la obtinerea handle proces");
    }


    ULONG processdefaultcpuSetsInformation[256];
    ULONG numberofCpuSets;
    if (!GetProcessDefaultCpuSets(currentProcessHandle, processdefaultcpuSetsInformation,
                                  sizeof(processdefaultcpuSetsInformation), &numberofCpuSets)) {
        printf("Eroare");
    }
    sprintf(buffer, "Number of cpu sets:%d\n", numberofCpuSets);
    write_in_file(path, buffer);
    printf("Number of cpu sets:%d\n", numberofCpuSets);
    for (ULONG i = 0; i < numberofCpuSets; i++) {
        sprintf(buffer, "Set id %d\n", processdefaultcpuSetsInformation[i]);
        write_in_file(path, buffer);
        printf("Set id %d\n", processdefaultcpuSetsInformation[i]);
    }

    //GetSystemCpuSetInformation
    printf("\n");

    SYSTEM_CPU_SET_INFORMATION systemCpuSetInformation[256];
    ULONG returnLength;
    if (systemCpuSetInformation == NULL) {
        printf("Eroare la alocarea memoriei");
    }
    ULONG cpuIndex = 0;
    int ok = 1;
    if (!GetSystemCpuSetInformation(systemCpuSetInformation, sizeof(systemCpuSetInformation), &returnLength,
                                    currentProcessHandle, 0)) {
        printf("Eroare");
    } else {
        sprintf(buffer, "The number of Current Process default CPU sets:%d\n",
                returnLength / sizeof(SYSTEM_CPU_SET_INFORMATION));
        write_in_file(path, buffer);
        printf("The number of Current Process default CPU sets:%d\n",
               returnLength / sizeof(SYSTEM_CPU_SET_INFORMATION));
        for (ULONG i = 0; i < returnLength / sizeof(SYSTEM_CPU_SET_INFORMATION); i++) {
            sprintf(buffer, "Processor set Id: %d\n", systemCpuSetInformation[i].CpuSet.Id);
            write_in_file(path, buffer);
            printf("Processor set Id: %d\n", systemCpuSetInformation[i].CpuSet.Id);
            sprintf(buffer, "Processor set Group: %d\n", systemCpuSetInformation[i].CpuSet.Group);
            write_in_file(path, buffer);
            printf("Processor set Group: %d\n", systemCpuSetInformation[i].CpuSet.Group);
            sprintf(buffer, "Processor set Logical Processor index: %d\n",
                    systemCpuSetInformation[i].CpuSet.LogicalProcessorIndex);
            write_in_file(path, buffer);
            printf("Processor set Logical Processor index: %d\n",
                   systemCpuSetInformation[i].CpuSet.LogicalProcessorIndex);
            sprintf(buffer, "Processor set core index: %d\n", systemCpuSetInformation[i].CpuSet.LogicalProcessorIndex);
            write_in_file(path, buffer);
            printf("Processor set core index: %d\n", systemCpuSetInformation[i].CpuSet.LogicalProcessorIndex);
            sprintf(buffer, "Processor set last level cache index: %d\n",
                    systemCpuSetInformation[i].CpuSet.LastLevelCacheIndex);
            write_in_file(path, buffer);
            printf("Processor set last level cache index: %d\n", systemCpuSetInformation[i].CpuSet.LastLevelCacheIndex);
            sprintf(buffer, "Processor set numa node index: %d\n", systemCpuSetInformation[i].CpuSet.NumaNodeIndex);
            write_in_file(path, buffer);
            printf("Processor set numa node index: %d\n", systemCpuSetInformation[i].CpuSet.NumaNodeIndex);
            sprintf(buffer, "Processor set efficiency class: %d\n", systemCpuSetInformation[i].CpuSet.EfficiencyClass);
            write_in_file(path, buffer);
            printf("Processor set efficiency class: %d\n", systemCpuSetInformation[i].CpuSet.EfficiencyClass);
            printf("\n");
        }
    }

}

int main() {
    GetComputerInfoHT();
    getComputerInfoNUMA();
    getComputerInfoCPUSets();
    //printf("Functioneaza");
    return 0;
}
