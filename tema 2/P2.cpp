#include <iostream>
#include <windows.h>

using namespace std;

DWORD WINAPI threadFunction(LPVOID param)
{
    DWORD path;
    char returnedPath[255] = {0};
    HANDLE handle;
    handle = CreateFileA((LPCSTR)param, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {

        printf("Eroare la deschiderea fisierului");
        CloseHandle(handle);
        return 0;
    }
    else
    {
        LARGE_INTEGER fileSize;
        int result;
        char *resultText;
        if (GetFileSizeEx(handle, &fileSize))
        {
            result = fileSize.LowPart;
        }

        // read from file
        // continue
        DWORD read;
        char ReadBuffer[1000000] = {0};
        if (ReadFile(handle, ReadBuffer, (result / sizeof(char)), &read, NULL) == FALSE)
        {
            printf("Eroare la citirea din fisier");
        }
        else
        {
            // print all the info

            // name
            char delim[] = "\\";
            char *word = (char *)param;
            char *fileName;
            char *f;
            //get the filename
            fileName = strtok(word, delim);
            while (fileName != NULL)
            {
                f = fileName;
                fileName = strtok(NULL, delim);
            }

            int numberOfSpace = 0;
            for (int i = 0; i < strlen(ReadBuffer); i++)
            {
                if (ReadBuffer[i] == 10)
                {
                    numberOfSpace++;
                }
            }
            printf("Numele fisierului: %s  Spatii: %d \n", f, numberOfSpace);
        }
        CloseHandle(handle);
        return 1;
    }
}

void executeThreads(char *path1, char *path2, char *path3)
{
    HANDLE t1, t2, t3;
    DWORD idt1, idt2, idt3;
    HANDLE vector[5];
    t1 = CreateThread(NULL, 0, threadFunction, path1, 0, &idt1);
    t2 = CreateThread(NULL, 0, threadFunction, path2, 0, &idt2);
    t3 = CreateThread(NULL, 0, threadFunction, path3, 0, &idt3);
    vector[0] = t1;
    vector[1] = t2;
    vector[2] = t3;
    if (t1 == INVALID_HANDLE_VALUE)
    {
        printf("Eroare la crearea threadului 1");
    }
    if (t2 == INVALID_HANDLE_VALUE)
    {
        printf("Eroare la crearea threadului 2");
    }
    if (t3 == INVALID_HANDLE_VALUE)
    {
        printf("Eroare la crearea threadului 3");
    }
    WaitForMultipleObjects(3, vector, TRUE, INFINITE);
    printf("Au fost executate toate");
    CloseHandle(t1);
    CloseHandle(t2);
    CloseHandle(t3);
}

int main()
{
    char path1[50];
    strcpy(path1, "C:\\Facultate\\CSSO\\Laboratoare\\Week2\\fire.txt");
    char path2[50];
    strcpy(path2, "C:\\Facultate\\CSSO\\Laboratoare\\Week2\\procese.txt");
    char path3[100];
    strcpy(path3, "C:\\Facultate\\CSSO\\Laboratoare\\Week2\\module_process.txt");
    executeThreads(path1, path2, path3);
    return 0;
}