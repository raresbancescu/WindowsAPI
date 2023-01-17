#include <iostream>
#include <Windows.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
using namespace std;

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
            printf("Nu s a putut crea folderul");
        }
        strcat(currentPath, "\\");
        strcat(currentPath, "\\");
        word = strtok(NULL, delim);
    }
}

void createFile(const char *fileName, char path[])
{
    HANDLE handle;
    printf("calea trimisa ca parametru: %s\n", path);
    char filePath[300];
    strcpy(filePath, path);
    strcat(filePath, "\\");
    strcat(filePath, "\\");
    strcat(filePath, fileName);
    printf("Path %s", filePath);
    handle = CreateFileA(filePath, GENERIC_ALL, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {
        printf("Eroare la creearea fisierului");
        CloseHandle(handle);
    }
    else
    {
        printf("S a reusit crearea fisierului %s \n", fileName);
        CloseHandle(handle);
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
            printf("S-au scris toate literele");
            CloseHandle(handle);
        }
    }
}



void enumerateSubKeysAndInfo(HKEY hKey, char *path, char *key)
{
    char finalPath[255];
    strcpy(finalPath, path);
    strcat(finalPath, "\\");
    strcat(finalPath, key);
    writeInFile(finalPath, "Subkey values:");
    long error;
    HKEY result;
    int numberOfKeys = 0;
    CHAR subkeysResult[255];
    DWORD subkeySize = 255;
    error = RegOpenKeyExA(hKey, NULL, NULL, KEY_ENUMERATE_SUB_KEYS, &result); // get a key(open)
    if (error == 0)
    {
        // succes: we have the key, now we get all the subkeys names
        //  IpcSubKeys
        while ((error = RegEnumKeyExA(result, numberOfKeys, subkeysResult, &subkeySize, NULL, NULL, NULL, NULL)) == ERROR_SUCCESS) // enumerate the subkeys
        {
            subkeySize = sizeof(subkeysResult);
            numberOfKeys++;
            // cout<<numberOfKeys<<" "<<subkeysResult<<endl;
            writeInFile(finalPath, subkeysResult);
            writeInFile(finalPath, " ");
        }

        writeInFile(finalPath, "\n");
        // Ipc MaxSubKeyLen
        // IpftLastWriteTime
        DWORD maxSubkeyLength;
        FILETIME lastWriteTime;
        SYSTEMTIME humanTime;
        char charTime[100];
        error = RegQueryInfoKeyA(result, NULL, NULL, NULL, NULL, &maxSubkeyLength, NULL, NULL, NULL, NULL, NULL, &lastWriteTime);
        writeInFile(finalPath, "Max subkey length: ");
        char charKeyLength[30];
        sprintf(charKeyLength, "%d", maxSubkeyLength);
        writeInFile(finalPath, charKeyLength);
        writeInFile(finalPath, "\n");

        if (FileTimeToSystemTime(&lastWriteTime, &humanTime))
        {
            sprintf(charTime, "Last Write Time: %d.%d.%d", humanTime.wDay, humanTime.wMonth, humanTime.wYear);
        }
        writeInFile(finalPath, charTime);
        RegCloseKey(hKey);
        RegCloseKey(result);
    }
}


void createRegistryKey()
{
    HKEY key;
    LPCTSTR directory = TEXT("SOFTWARE");

    if (RegOpenKeyEx(HKEY_CURRENT_USER, directory, 0, KEY_SET_VALUE, &key) != ERROR_SUCCESS)
    {
        printf("Eroare la cheie");
        printf("%u", GetLastError);
    }
    else
    {

        const char *lpValueName;
        BYTE Data[4];
        DWORD dwDisposition;
        HKEY phkResult;
        HKEY phk2Result;

        if (!RegCreateKeyExA(key, "CSSO", 0, "", 0, KEY_ALL_ACCESS, 0, &phkResult, &dwDisposition))
        {
            if (!RegCreateKeyExA(phkResult, "Week1", 0, "", 0, KEY_ALL_ACCESS, 0, &phk2Result, &dwDisposition))
            {
                HANDLE handle;
                handle = CreateFileA("C:\\Facultate\\CSSO\\Laboratoare\\Week1\\Rezultate\\sumar.txt", GENERIC_READ, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                if (handle == INVALID_HANDLE_VALUE)
                {

                    printf("Eroare la deschiderea fisierului");
                    CloseHandle(handle);
                }
                else
                {
                    printf("S a deschis fisierul");
                    LARGE_INTEGER fileSize;

                    if (GetFileSizeEx(handle, &fileSize))
                    {
                        char result[25];
                        sprintf(result, "%d", fileSize.HighPart);
                        LPCTSTR keyValue0 = (LPCTSTR)result;
                        
                        if (RegSetValueEx(phk2Result, TEXT("size"), 0, REG_DWORD, (const BYTE *)keyValue0, (lstrlen(keyValue0)) + 1) != ERROR_SUCCESS)
                        {
                            printf("Nu s a reusit setarea valorii in cheie1");
                            RegCloseKey(phk2Result);
                        }
                        else
                        {
                            RegCloseKey(phk2Result);
                        }
                    }
                    CloseHandle(handle);
                }

                LPCTSTR keyValue = TEXT("C:\\Facultate\\CSSO\\Laboratoare\\Week1\\Rezultate\\sumar.txt");
                if (RegSetValueEx(phk2Result, TEXT("path"), 0, REG_SZ, (const BYTE *)keyValue, (lstrlen(keyValue)) + 1) != ERROR_SUCCESS)
                {
                    printf("Nu s a reusit setarea valorii in cheie");
                    RegCloseKey(phk2Result);
                }
                else
                {
                    RegCloseKey(phk2Result);
                }
            }
            RegCloseKey(phkResult);
        }
        RegCloseKey(key);
    }
}

void displayPathAndSize(const char *fileName) // write in file and solve the size problem
{
    DWORD path;
    char returnedPath[255] = {0};
    char *file; // just a pointer
    HANDLE handle;
    path = GetFullPathNameA(fileName, 255, returnedPath, &file);
    writeInFile("C:\\Facultate\\CSSO\\Laboratoare\\Week1\\Rezultate\\sumar.txt", "\n");
    writeInFile("C:\\Facultate\\CSSO\\Laboratoare\\Week1\\Rezultate\\sumar.txt", returnedPath);
    handle = CreateFileA(returnedPath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {

        printf("Eroare la deschiderea fisierului");
        CloseHandle(handle);
    }
    else
    {
        printf("S a deschis fisierul");
        LARGE_INTEGER fileSize;

        if (GetFileSizeEx(handle, &fileSize))
        {
            char result[25];
            sprintf(result, "%d", fileSize.LowPart);
            writeInFile("C:\\Facultate\\CSSO\\Laboratoare\\Week1\\Rezultate\\sumar.txt", "\nFile size: ");
            writeInFile("C:\\Facultate\\CSSO\\Laboratoare\\Week1\\Rezultate\\sumar.txt", result);
        }
        CloseHandle(handle);
    }
}

int main()
{
    char firstPath[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week1";
    char secondPath[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week1\\Extensii";
    char thirdPath[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week1\\Rezultate";
    char filesPath[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week1\\Rezultate";
    createDirectory(firstPath);
    createDirectory(secondPath);
    createDirectory(thirdPath);

    enumerateSubKeysAndInfo(HKEY_LOCAL_MACHINE,filesPath,"HKLM.txt");
    enumerateSubKeysAndInfo(HKEY_CURRENT_CONFIG,filesPath,"HKCC.txt");
    enumerateSubKeysAndInfo(HKEY_CURRENT_USER,filesPath,"HKCU.txt");
    sleep(1);
    displayPathAndSize("C:\\Facultate\\CSSO\\Laboratoare\\Week1\\Rezultate\\HKCC.txt");
    displayPathAndSize("C:\\Facultate\\CSSO\\Laboratoare\\Week1\\Rezultate\\HKCU.txt");
    displayPathAndSize("HKCC.txt");
    
   // createRegistryKey();
    return 0;
}