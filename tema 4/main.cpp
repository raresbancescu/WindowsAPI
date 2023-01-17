#pragma comment(lib, "Wininet")
#include <iostream>
#include <wininet.h>
#include <string>
using namespace std;

// g++ main.cpp -lwininet -o main
int totalSize = 0;
int totalRequests = 0;
int totalGet = 0;
int totalPost = 0;
char *path = "http://cssohw.herokuapp.com";
char *path2 = "www.google.com";
char directoryPath[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week4";
char getDirectoryPath[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week4\\Downloads";
char *filePath = "C:\\Facultate\\CSSO\\Laboratoare\\Week4\\myconfig.txt";
char valueOfLastGet[100] = "";
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

void httpInitialRequest(char *serverName)
{
    HINTERNET hInternet = InternetOpenA("310910401RSL201028", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

    if (hInternet == NULL)
    {
        printf("InternetOpen failed %s", GetLastError());
    }
    else
    {

        HINTERNET hConnect = InternetConnectA(hInternet, "cssohw.herokuapp.com", 80, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        if (hConnect == NULL)
        {
            printf("Internet Connect failed %s", GetLastError());
        }
        else
        {

            LPCSTR acceptTypes[] = {"text/*", NULL};
            HINTERNET request = HttpOpenRequestA(hConnect, "GET", "/assignhomework/310910401RSL201028", NULL, NULL, acceptTypes, 0, 0);
            if (request == NULL)
            {
                printf("Open request failed %s", GetLastError());
            }
            else
            {

                BOOL requestSend = HttpSendRequestA(request, NULL, 0, 0, 0);

                if (requestSend == 0)
                {
                    printf("Error at send request");
                }
                else
                {

                    char finalText[1024] = "";
                    int bufferSize = 1024;
                    char whatIsRead[bufferSize];
                    int ok = 1;
                    DWORD numberOfReadBites = -1;

                    ok = InternetReadFile(request, whatIsRead, bufferSize, &numberOfReadBites);
                    // printf("%s", whatIsRead);
                    writeInFile(filePath, whatIsRead);
                    memset(whatIsRead, 0, bufferSize);
                    strcpy(whatIsRead, "");
                }
                InternetCloseHandle(request);
            }
            InternetCloseHandle(hConnect);
        }

        InternetCloseHandle(hInternet);
    }
}

void httpGetRequest(char *link)
{
    char path[10] = "";
    int index = 0;
    printf("Get on: %s\n", link);
    for (int i = 0; i < strlen(link); i++)
    {
        if (link[i] <= '9' && link[i] >= '0')
        {
            path[index++] = link[i];
        }
    }
    char getFilePath[100] = ""; // creating the file at this absolute path
    strcpy(getFilePath, "C:\\Facultate\\CSSO\\Laboratoare\\Week4\\Downloads\\");
    strcat(getFilePath, path);
    // printf("%s", getFilePath);
    // printf(" %s\n", path);

    HINTERNET hInternet = InternetOpenA("310910401RSL201028", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

    if (hInternet == NULL)
    {
        printf("InternetOpen failed %s", GetLastError());
    }
    else
    {

        HINTERNET hConnect = InternetConnectA(hInternet, "cssohw.herokuapp.com", 80, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        if (hConnect == NULL)
        {
            printf("Internet Connect failed %s", GetLastError());
        }
        else
        {

            LPCSTR acceptTypes[] = {"text/*", NULL};
            strcpy(link, link + 20);
            HINTERNET request = HttpOpenRequestA(hConnect, "GET", link, NULL, NULL, acceptTypes, 0, 0);
            if (request == NULL)
            {
                printf("Open request failed %s", GetLastError());
            }
            else
            {

                BOOL requestSend = HttpSendRequestA(request, NULL, 0, 0, 0);

                if (requestSend == 0)
                {
                    printf("Error at send request");
                }
                else
                {

                    char finalText[1024] = "";
                    int bufferSize = 1024;
                    char whatIsRead[bufferSize];
                    int ok = 1;
                    DWORD numberOfReadBites = -1;

                    ok = InternetReadFile(request, whatIsRead, bufferSize, &numberOfReadBites);
                    // printf("%s", whatIsRead);

                    strcat(valueOfLastGet, whatIsRead);
                    strcat(whatIsRead, "\n");
                    writeInFile(getFilePath, whatIsRead);
                    memset(whatIsRead, 0, bufferSize);
                }
                InternetCloseHandle(request);
            }
            InternetCloseHandle(hConnect);
        }

        InternetCloseHandle(hInternet);
    }
}

void getAllFilesSizes()
{
    char fileNames[1000][30];
    int index = 0;
    WIN32_FIND_DATA data;
    HANDLE handle;
    handle = FindFirstFileA("C:\\Facultate\\CSSO\\Laboratoare\\Week4\\Downloads\\*", &data);
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
        char folderPath[100] = "C:\\Facultate\\CSSO\\Laboratoare\\Week4\\Downloads\\";
        // printf("%s\n", fileNames[i]);
        char returnedPath[100];
        strcpy(returnedPath, folderPath);
        strcat(returnedPath, fileNames[i]);
        HANDLE handle;
        handle = CreateFileA(returnedPath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (handle == INVALID_HANDLE_VALUE)
        {

            // printf("Eroare la deschiderea fisierului");
            CloseHandle(handle);
        }
        else
        {
            LARGE_INTEGER fileSize;

            if (GetFileSizeEx(handle, &fileSize))
            {

                totalSize += fileSize.LowPart;
            }
            CloseHandle(handle);
        }
    }
    printf("\nTotal size is %d", totalSize);
}

void httpPostRequest(char *link)
{
    char path[10] = "";
    printf("Post on: %s\n", link);
    int index = 0;
    for (int i = 0; i < strlen(link); i++)
    {
        if (link[i] <= '9' && link[i] >= '0')
        {
            path[index++] = link[i];
        }
    }
    char getFilePath[100] = ""; // creating the file at this absolute path
    strcpy(getFilePath, "C:\\Facultate\\CSSO\\Laboratoare\\Week4\\Downloads\\");
    strcat(getFilePath, path);

    HINTERNET hInternet = InternetOpenA("310910401RSL201028", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

    if (hInternet == NULL)
    {
        printf("InternetOpen on post failed %s", GetLastError());
    }
    else
    {

        HINTERNET hConnect = InternetConnectA(hInternet, "cssohw.herokuapp.com", 80, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);
        if (hConnect == NULL)
        {
            printf("Internet Connect on post failed %s", GetLastError());
        }
        else
        {

            strcpy(link, link + 20);
            HINTERNET request = HttpOpenRequestA(hConnect, "POST", link, NULL, NULL, NULL, 0, 1);
            if (request == NULL)
            {
                printf("Open request failed %s", GetLastError());
            }
            else
            {
                char headers[] = "Content-Type: application/x-www-form-urlencoded";
                char dataToPost[] = "id=310910401RSL201028&value="; // modify
                strcat(dataToPost, valueOfLastGet);
                cout << dataToPost;
                BOOL requestSend = HttpSendRequestA(request, headers, strlen(headers), dataToPost, strlen(dataToPost));
                if (requestSend == 0)
                {
                    printf("Error at send request");
                }
                else
                {

                    char finalText[1024] = {0};
                    int bufferSize = 1024;
                    char whatIsRead[bufferSize] = {0};
                    int ok = 1;
                    DWORD numberOfReadBites = -1;

                    ok = InternetReadFile(request, whatIsRead, bufferSize, &numberOfReadBites);
                    if (strstr(whatIsRead, "received"))
                        printf("%s", "Post worked\n");
                    strcpy(whatIsRead, "");
                }
                InternetCloseHandle(request);
            }
            InternetCloseHandle(hConnect);
        }

        InternetCloseHandle(hInternet);
    }
}

void readFromFile(char *param)
{
    createDirectory(getDirectoryPath);
    HANDLE handle;
    handle = CreateFileA((LPCSTR)param, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {

        // printf("Eroare la deschiderea fisierului");
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
            fflush(stdin);
        }
        else
        {
            char *value;

            value = strtok(readBuffer, "\n");
            while (value)
            {
                totalRequests += 1;
                if (strstr(value, "GET"))
                {
                    totalGet += 1;
                    strcpy(value, value + 11);
                    strcpy(valueOfLastGet, "");
                    httpGetRequest(value);
                }
                else if (strstr(value, "POST"))
                {
                    totalPost += 1;
                    strcpy(value, value + 12);

                    httpPostRequest(value);
                }
                value = strtok(NULL, "\n");
            }
        }
        CloseHandle(handle);
    }
    printf("\nAll: %d Get: %d Post %d\n", totalRequests, totalGet, totalPost);
}

void finalHttpPostRequest()
{

    HINTERNET hInternet = InternetOpenA("310910401RSL201028", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

    if (hInternet == NULL)
    {
        printf("InternetOpen on final post failed %s", GetLastError());
    }
    else
    {

        HINTERNET hConnect = InternetConnectA(hInternet, "cssohw.herokuapp.com", 80, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);
        if (hConnect == NULL)
        {
            printf("Internet Connect on final post failed %s", GetLastError());
        }
        else
        {

            HINTERNET request = HttpOpenRequestA(hConnect, "POST", "/endhomework", NULL, NULL, NULL, 0, 1);
            if (request == NULL)
            {
                printf("Final open request failed %s", GetLastError());
            }
            else
            {
                char headers[] = "Content-Type: application/x-www-form-urlencoded";
                char dataToPost[100]; // modify
                sprintf(dataToPost, "id=310910401RSL201028&total=%d&get=%d&post=%d&size=%d", totalRequests, totalGet, totalPost, totalSize);
                BOOL requestSend = HttpSendRequestA(request, headers, strlen(headers), dataToPost, strlen(dataToPost));
                if (requestSend == 0)
                {
                    printf("Error at send request");
                }
                else
                {

                    char finalText[1024] = {0};
                    int bufferSize = 1024;
                    char whatIsRead[bufferSize] = {0};
                    int ok = 1;
                    DWORD numberOfReadBites = -1;

                    ok = InternetReadFile(request, whatIsRead, bufferSize, &numberOfReadBites);
                    printf("\nAsta am primit la requestul final: %s", whatIsRead);
                    strcpy(whatIsRead, "");
                }
                InternetCloseHandle(request);
            }
            InternetCloseHandle(hConnect);
        }

        InternetCloseHandle(hInternet);
    }
}

int main()
{
    createDirectory(directoryPath);
    httpInitialRequest(path);
    readFromFile(filePath);
    getAllFilesSizes();
    finalHttpPostRequest();
    return 0;
}