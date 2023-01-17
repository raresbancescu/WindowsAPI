#include <iostream>
#include <windows.h>

using namespace std;

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

void writeInFileNewValue(const char *path, char *text)
{

    HANDLE handle;
    BOOL ErrorFlag;
    DWORD NumberOfLetters = (DWORD)strlen(text);
    DWORD NumberOfLettersWritten = 0;
    handle = CreateFileA(path, GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
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

void changeValueInPayments(char *value)
{
    HANDLE handle;
    int fileExists = 1;
    char path[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports\\Summary\\payments.txt";
    handle = CreateFileA((LPCSTR)path, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {

        printf("Eroare la deschiderea fisierului");
        fileExists = 0;
        CloseHandle(handle);
    }
    else
    {
        DWORD read;
        LARGE_INTEGER fileSize;
        char readBuffer[100] = {0};
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
            int number = atoi(readBuffer);
            number = number - atoi(value);
            char finalSummary[100];
            sprintf(finalSummary, "%d", number);
            CloseHandle(handle);
            writeInFileNewValue("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports\\Summary\\payments.txt", finalSummary);
        }
        CloseHandle(handle);
    }
    if (fileExists == 0)
    {
        char finalSummary[10];
        strcpy(finalSummary, "0");
        writeInFile("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports\\Summary\\payments.txt", finalSummary);
    }
}

void changeValueInSummary(char *value)
{
    HANDLE handle;
    int fileExists = 1;
    char path[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports\\Summary\\summary.txt";
    handle = CreateFileA((LPCSTR)path, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {

        printf("Eroare la deschiderea fisierului");
        fileExists = 0;
        CloseHandle(handle);
    }
    else
    {
        DWORD read;
        LARGE_INTEGER fileSize;
        char readBuffer[100] = {0};
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
            int number = atoi(readBuffer);
            number = number - atoi(value);
            char finalSummary[100];
            sprintf(finalSummary, "%d", number);
            CloseHandle(handle);
            writeInFileNewValue("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports\\Summary\\summary.txt", finalSummary);
        }
        CloseHandle(handle);
    }
    if (fileExists == 0)
    {
        char finalSummary[10];
        strcpy(finalSummary, "0");
        writeInFile("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports\\Summary\\summary.txt", finalSummary);
    }
}

void MappingWrite(char *message)
{
    HANDLE mapping;
    TCHAR fileName[] = TEXT("cssohw3management");
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
            // printf("Eroare la obtinerea adresei memoriei");
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

void readFromMemoryAndChangeValue()
{
    HANDLE handle;
    int fileExists = 1;
    int income;
    char path[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports\\Summary\\income.txt";
    handle = CreateFileA((LPCSTR)path, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {

        printf("Eroare la deschiderea fisierului");
        fileExists = 0;
        CloseHandle(handle);
    }
    else
    {
        DWORD read;
        LARGE_INTEGER fileSize;
        char readBuffer[100] = {0};
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
            income = atoi(readBuffer);
        }
        CloseHandle(handle);
    }

    HANDLE mapping;
    int number;
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
            // printf("Eroare la obtinerea adresei memoriei");
            UnmapViewOfFile(read);
        }
        else
        {
            number = atoi(read);
            number -= income;
            UnmapViewOfFile(read);
        }
        CloseHandle(mapping);
        char whatToWrite[100];
        sprintf(whatToWrite, "%d", number);
        MappingWrite(whatToWrite);
    }
}

void readFromFile(char *param)
{
    HANDLE handle;
    char path[] = "D:\\facultate\\Anul 3\\Semestrul 1\\CSSO\\tema 3\\H3_input\\payments\\";
    strcat(path, param);
    // printf("%s\n",path);
    handle = CreateFileA((LPCSTR)path, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {

        //printf("Eroare la deschiderea fisierului");
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
            // we have the informations in the buffer
            int totalSum = 0;
            char *value;
            int nr = 0;
            char textToWriteInFile[100];
            value = strtok(readBuffer, "\n");
            while (value)
            {
                strcpy(textToWriteInFile, "");
                sprintf(textToWriteInFile, "S-a facut o plata de %s in data de %s\n", value, param);
                writeInFile("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports\\logs.txt", textToWriteInFile);

                totalSum += atoi(value);

                value = strtok(NULL, "\n");
            }
            // create the file
            // subpunct 2
            char filePath[100];
            strcpy(filePath, "C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports\\Daily\\");
            strcat(filePath, param);
            strcat(filePath, "_payments.txt");
            char result[10];
            sprintf(result, "%d", totalSum);
            // printf("Sum %d",totalSum);
            writeInFile(filePath, result);

            // subpunct 3

            changeValueInPayments(result);
            changeValueInSummary(result);
        }
        CloseHandle(handle);
    }
}

void writeLogs()
{
    char fileNames[1000][30];
    int index = 0;
    WIN32_FIND_DATA data;
    HANDLE handle;
    handle = FindFirstFile("D:\\facultate\\Anul 3\\Semestrul 1\\CSSO\\tema 3\\H3_input\\payments\\*", &data);
    if (handle == INVALID_HANDLE_VALUE)
    {
        printf("Eroare");
        FindClose(handle);
    }
    else
    {
        do
        {
            sprintf(fileNames[index], "%s", data.cFileName);
            index++;
        } while (FindNextFile(handle, &data));
        FindClose(handle);
    }

    for (int i = 0; i < index; i++)
    {
        HANDLE handle;
        handle = OpenMutex(SYNCHRONIZE, false, "mutex");
        WaitForSingleObject(handle, INFINITE);
        readFromFile(fileNames[i]);
        ReleaseMutex(handle);
    }
}

int main()
{
    cout << "Sunt procesul pay";
    
    HANDLE semaphoreHandle;
   
    semaphoreHandle = OpenSemaphore(SEMAPHORE_MODIFY_STATE, false, "semafor");

    writeLogs();

    WaitForSingleObject(semaphoreHandle, INFINITE);
    readFromMemoryAndChangeValue();
    ReleaseSemaphore(semaphoreHandle, 1, NULL);
    return 0;
}