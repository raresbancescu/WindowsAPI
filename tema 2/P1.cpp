#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <winbase.h>
using namespace std;

const char *processPath = "C:\\Facultate\\CSSO\\Laboratoare\\Week2\\procese.txt";
const char *threadPath = "C:\\Facultate\\CSSO\\Laboratoare\\Week2\\fire.txt";
const char *moduleProcessPath = "C:\\Facultate\\CSSO\\Laboratoare\\Week2\\module_process.txt";
char directoryPath[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week2";

// number of processes modules and threads
int numberOfThreads = 0;
int numberOfModules = 0;
int numberOfProcesses = 0;

void createDirectory(char path[])
{
    char delim[] = "\\";
    char *word = strtok(path, delim);
    char currentPath[100] = {0};
    strcpy(currentPath, word);
    strcat(currentPath, "\\");
    strcat(currentPath, "\\");
    word = strtok(NULL, delim);
    while (word != NULL)
    {

        strcat(currentPath, word);
        if (CreateDirectoryA(currentPath, NULL) == 0)
        {
            // printf("Nu s a putut crea folderul");
        }
        strcat(currentPath, "\\");
        strcat(currentPath, "\\");
        word = strtok(NULL, delim);
    }
}

void writeInFile(const char *path, char *text)
{

    HANDLE handle;
    BOOL ErrorFlag;
    DWORD NumberOfLetters = (DWORD)strlen(text);
    DWORD NumberOfLettersWritten = 0;
    handle = CreateFileA(path, FILE_APPEND_DATA, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {
        printf("Eroare la deschiderea fisierului");
        CloseHandle(handle);
    }

    ErrorFlag = WriteFile(handle, text, NumberOfLetters, &NumberOfLettersWritten, NULL);

    if (ErrorFlag == 0)
    {

        printf("Nu s e putut scrie in fisier");
        CloseHandle(handle);
    }
    else
    {
        if (NumberOfLetters != NumberOfLettersWritten)
        {
            printf("Nu s au putut scrie toate literele");
            CloseHandle(handle);
        }
        else
        {
            // printf("S-au scris toate literele");
            CloseHandle(handle);
        }
    }
}

void allProcesses()
{
    HANDLE snapshot;
    PROCESSENTRY32 process;
    process.dwSize = sizeof(PROCESSENTRY32); // set the size
    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapshot == INVALID_HANDLE_VALUE)
    {
        printf("Eroare la snapshot");
        CloseHandle(snapshot);
    }
    else
    {
        if (!Process32First(snapshot, &process))
        {
            printf("Gasirea primului proces a dat eroare");
        }
        else
        {
            do
            {
                char result[255];
                numberOfProcesses++;
                sprintf(result, "Parent process ID: %d   ", process.th32ParentProcessID);
                writeInFile(processPath, result);
                memset(result, 0, 255);
                sprintf(result, "Process ID: %d   ", process.th32ProcessID);
                writeInFile(processPath, result);
                memset(result, 0, 255);
                sprintf(result, "SzExeFile: %s \n", process.szExeFile);
                writeInFile(processPath, result);
            } while (Process32Next(snapshot, &process));
        }
        CloseHandle(snapshot);
    }
}

void modulesForCurrentProcess()
{
    HANDLE snapshot;
    MODULEENTRY32 modules;
    modules.dwSize = sizeof(MODULEENTRY32);
    DWORD processId = GetCurrentProcessId();
    // printf("Process id: %d\n",processId);
    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);

    if (snapshot == INVALID_HANDLE_VALUE)
    {
        printf("Eroare la snapshot");
        CloseHandle(snapshot);
    }
    else
    {
        if (!Module32First(snapshot, &modules))
        {
            printf("Gasirea primului modul a dat eroare");
        }
        else
        {
            do
            {
                numberOfModules++;
                char result[255];
                sprintf(result, "Module ID: %d   ", modules.th32ModuleID);
                // cout << result;
                writeInFile(moduleProcessPath, result);
                memset(result, 0, 255);
                sprintf(result, "Process ID: %d   ", modules.th32ProcessID);
                // cout << result;
                writeInFile(moduleProcessPath, result);
                memset(result, 0, 255);
                sprintf(result, "SzModule: %s ", modules.szModule);
                // cout << result;
                writeInFile(moduleProcessPath, result);
                memset(result, 0, 255);
                sprintf(result, "szExePath %s  \n", modules.szExePath);
                // cout << result;
                writeInFile(moduleProcessPath, result);
            } while (Module32Next(snapshot, &modules));
        }
        CloseHandle(snapshot);
    }
}

void allThreads()
{
    HANDLE snapshot;
    THREADENTRY32 thread;
    thread.dwSize = sizeof(THREADENTRY32); // set the size
    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    if (snapshot == INVALID_HANDLE_VALUE)
    {
        printf("Eroare la snapshot");
        CloseHandle(snapshot);
    }
    else
    {
        if (!Thread32First(snapshot, &thread))
        {
            printf("Gasirea primului thread a dat eroare");
        }
        else
        {
            do
            {
                numberOfThreads++;
                char result[255];
                sprintf(result, "Owner process Id: %d   ", thread.th32OwnerProcessID);
                writeInFile(threadPath, result);
                memset(result, 0, 255);
                sprintf(result, "Thread ID: %d\n", thread.th32ThreadID);
                writeInFile(threadPath, result);

            } while (Thread32Next(snapshot, &thread));
        }
        CloseHandle(snapshot);
    }
}

void MappingWrite()
{
    HANDLE mapping;
    char message[255];
    TCHAR fileName[] = TEXT("cssow2basicsync");
    sprintf(message, "Module: %d\nProcese: %d\nFire: %d\n", numberOfModules, numberOfProcesses, numberOfThreads);

    mapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, fileName);
    LPCTSTR getAddressOfMapping;
    if (mapping == INVALID_HANDLE_VALUE)
    {
        printf("Eroare la deschiderea maparii in memorie");
        CloseHandle(mapping);
    }
    else
    {
        getAddressOfMapping = (LPTSTR)MapViewOfFile(mapping, FILE_MAP_ALL_ACCESS, 0, 0, 256);
        if (getAddressOfMapping == NULL)
        {
            printf("Eroare la obtinerea adresei memoriei");
            UnmapViewOfFile(getAddressOfMapping);
        }
        else
        {
            CopyMemory((PVOID)getAddressOfMapping, message, (strlen(message) * sizeof(char)));
            UnmapViewOfFile(getAddressOfMapping);
        }
        CloseHandle(mapping);
    }
}

void ajustPrivilege()
{

    HANDLE tokenHandle;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &tokenHandle) == false)
    {
        printf("Eroare la deschiderea tokenului procesului");
    }
    else
    {
        LUID element;
        LPCTSTR privilegeName = SE_SYSTEM_PROFILE_NAME;
        if (LookupPrivilegeValue(NULL, privilegeName, &element) == false)
        {
            printf("Eroare la lookup");
        }
        else
        {
            TOKEN_PRIVILEGES privilegii;
            privilegii.PrivilegeCount = 1;
            privilegii.Privileges[0].Luid = element;
            privilegii.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

            if (AdjustTokenPrivileges(tokenHandle, FALSE, &privilegii, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL) == false)
            {
                printf("Eroare la setarea privilegiului\n");
                printf("Last error: %u\n", GetLastError());
            }
            else
            {
                printf("S a setat privilegiul\n");
            }
        }
        CloseHandle(tokenHandle);
    }
}

int main()
{
    ajustPrivilege();
    createDirectory(directoryPath);
    allProcesses();
    allThreads();
    modulesForCurrentProcess();
    MappingWrite();
    return 0;
}