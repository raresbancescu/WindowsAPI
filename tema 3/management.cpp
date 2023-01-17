#include <iostream>
#include <windows.h>

char reportPath[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports";
char dailyPath[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports\\Daily";
char summaryPath[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports\\Summary";

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

void CreateMapping()
{
    HANDLE mapping;
    mapping = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 256, "cssohw3management");
    if (mapping == INVALID_HANDLE_VALUE)
    {
        printf("Eroare la crearea mapping");
        CloseHandle(mapping);
    }
    // nu inchidem handle ul intentionat pentru a ramane deschisa zona de memorie
}

void MappingWrite()
{
    HANDLE mapping;
    char message[255];
    TCHAR fileName[] = TEXT("cssohw3management");
    sprintf(message, "0");

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
        //CloseHandle(mapping);
    }
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

void readFromMemory()
{
    HANDLE mapping;
    TCHAR fileName[] = TEXT("cssohw3management");
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
            //UnmapViewOfFile(read);
        }
        else
        {
            printf("Memorie: %s\n", read);
            UnmapViewOfFile(read);
        }
       CloseHandle(mapping);
    }
}

void readFromFile(const char *param)
{
    HANDLE handle;
    handle = CreateFileA((LPCSTR)param, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {

        printf("Eroare la deschiderea fisierului");
        CloseHandle(handle);
    }
    else
    {
        DWORD read;
        LARGE_INTEGER fileSize;
        char readBuffer[100000] = {0};
        int result;
        if (GetFileSizeEx(handle, &fileSize))
        {
            result = fileSize.LowPart;
        }
        if (ReadFile(handle, readBuffer, (result / sizeof(char)), &read, NULL) == FALSE)
        {
            printf("Eroare la citirea din fisier");
        }
        else
        {
            printf("%s\n", readBuffer);
        }
        CloseHandle(handle);
    }
}

void readAllFiles()
{
    readFromFile("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports\\Summary\\income.txt");
    readFromFile("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports\\Summary\\payments.txt");
    readFromFile("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports\\Summary\\summary.txt");
}

void createMutex()
{
    HANDLE handle;
    handle = CreateMutex(NULL, false, "mutex");
}

void createSemaphore()
{
    HANDLE handle;
    handle = CreateSemaphore(NULL, 0, 2, "semafor");
}

void createMutex2()
{
    HANDLE handle;
    handle = CreateMutex(NULL, false, "mainMutex");
}



int main()
{
    createDirectory(reportPath);
    createDirectory(dailyPath);
    createDirectory(summaryPath);
    createMutex();
    createMutex2();
    createSemaphore();
    CreateMapping();
    MappingWrite();
    createProcess("D:\\facultate\\Anul 3\\Semestrul 1\\CSSO\\tema 3\\income.exe");
    createProcess("D:\\facultate\\Anul 3\\Semestrul 1\\CSSO\\tema 3\\pay.exe");
    createProcess("D:\\facultate\\Anul 3\\Semestrul 1\\CSSO\\tema 3\\generate.exe");
    HANDLE handle;
    handle=OpenMutex(SYNCHRONIZE,false,"mutex2");
    WaitForSingleObject(handle,INFINITE);
    readFromMemory();
    readAllFiles();
    ReleaseMutex(handle);
    return 0;
}