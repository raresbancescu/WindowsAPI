#include <iostream>
#include <windows.h>
int totalIncome = 0;
int totalPayments = 0;

void writeInFile(const char *path, char *text)
{

    HANDLE handle;
    BOOL ErrorFlag;
    DWORD NumberOfLetters = (DWORD)strlen(text);
    DWORD NumberOfLettersWritten = 0;
    handle = CreateFileA(path, FILE_APPEND_DATA, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {
        // printf("Eroare la deschiderea fisierului");
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

void readFromFile(char *param)
{
    HANDLE handle;
    char path2[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports\\Daily\\";
    char path[101];
    sprintf(path, "%s%s", path2, param);
    handle = CreateFileA((LPCSTR)path, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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
            // we have the informations in the buffer
            if (strstr(param, "_income"))
            {
                char textToWriteInFile[100];
                totalIncome += atoi(readBuffer);
                sprintf(textToWriteInFile, "S-au facut plati de %d\n", totalPayments);
                writeInFile("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports\\logs.txt", textToWriteInFile);
            }
            else
            {
                char textToWriteInFile2[100];
                sprintf(textToWriteInFile2, "S-au facut incasari de %d\n", totalIncome);
                totalPayments += atoi(readBuffer);
                writeInFile("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports\\logs.txt", textToWriteInFile2);
            }

            char textToWriteInFile3[100];
            sprintf(textToWriteInFile3, "S-a realizat un profit de %d\n", totalPayments + totalIncome);
            writeInFile("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports\\logs.txt", textToWriteInFile3);
        }

        CloseHandle(handle);
    }
}

void readFromMemoryAndCompare()
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
            UnmapViewOfFile(read);
        }
        else
        {
            printf("%s", read);
            printf("Cat e read:%s", read);
            char textToWriteInFile[100];

            if (atoi(read) == (totalIncome - totalPayments))
            {
                strcpy(textToWriteInFile, "Raport generat cu success!");
                writeInFile("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports\\logs.txt", textToWriteInFile);
            }

            else
            {
                strcpy(textToWriteInFile, "Ai o greșeală la generarea raportului");
                writeInFile("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports\\logs.txt", textToWriteInFile);
            }

            UnmapViewOfFile(read);
        }
        CloseHandle(mapping);
    }
}

void writeLogs()
{
    char fileNames[1000][30];
    int index = 0;
    WIN32_FIND_DATA data;
    HANDLE handle;
    handle = FindFirstFile("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports\\Daily\\*", &data);
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
        // printf("%s",fileNames[i]);
        HANDLE handle;
        handle = OpenMutex(SYNCHRONIZE, false, "mainMutex");
        WaitForSingleObject(handle, INFINITE);
        readFromFile(fileNames[i]);
        ReleaseMutex(handle);
    }
    char textToWriteInFile[200];
    strcpy(textToWriteInFile, "");
    sprintf(textToWriteInFile, "S-au facut incasari de %d\nS-au facut cheltuieli de %d\n S-a realizat un profit de %d\n", totalIncome, totalPayments, totalIncome - totalPayments);
    writeInFile("C:\\Facultate\\CSSO\\Laboratoare\\Week3\\Reports\\logs.txt", textToWriteInFile);
}

int main()
{
    writeLogs();
    HANDLE handle;
    handle = OpenMutex(SYNCHRONIZE, false, "mutex2");
    WaitForSingleObject(handle, INFINITE);
    readFromMemoryAndCompare();
    ReleaseMutex(handle);
    return 0;
}