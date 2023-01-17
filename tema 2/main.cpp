#include <windows.h>
#include <iostream>

using namespace std;

void CreateMapping()
{
    HANDLE mapping;
    mapping = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 256, "cssow2basicsync");
    if (mapping == INVALID_HANDLE_VALUE)
    {
        printf("Eroare la crearea mapping");
        CloseHandle(mapping);
    }
    // nu inchidem handle ul intentionat pentru a ramane deschisa zona de memorie
}

void createProcess(const char *processPath)
{
    STARTUPINFOA startupInfo;
    PROCESS_INFORMATION processInformation;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    ZeroMemory(&processInformation, sizeof(processInformation));
    startupInfo.cb = sizeof(startupInfo);
    if (!CreateProcessA(processPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInformation))
    {
        printf("Eroare la crearea procesului p1");
    }
    else
    {
        WaitForSingleObject(processInformation.hProcess, INFINITE);
        CloseHandle(processInformation.hProcess);
        CloseHandle(processInformation.hThread);
    }
}

void ReadFromMemory()
{
    HANDLE mapping;
    TCHAR fileName[] = TEXT("cssow2basicsync");
    LPCTSTR read;

    mapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, fileName);

    if (mapping == INVALID_HANDLE_VALUE)
    {
        printf("Eroare la deschiderea maparii in memorie");
        CloseHandle(mapping);
    }
    else
    {
        read = (LPTSTR)MapViewOfFile(mapping, FILE_MAP_ALL_ACCESS, 0, 0, 256);
        if (read == NULL)
        {
            printf("Eroare la obtinerea adresei memoriei");
            UnmapViewOfFile(read);
        }
        else
        {
            printf("%s", read);
            UnmapViewOfFile(read);
        }
        CloseHandle(mapping);
    }
}

int main()
{
    CreateMapping();
    createProcess("D:\\facultate\\Anul 3\\Semestrul 1\\CSSO\\tema 2\\P1.exe");
    ReadFromMemory();
    createProcess("D:\\facultate\\Anul 3\\Semestrul 1\\CSSO\\tema 2\\P2.exe");
    return 0;
}