#include <iostream>
#include <windows.h>
#include <sddl.h>

char path[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week5\\RESTRICTED_WRITE\\P2.txt";
char log_path[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week5\\LOGS\\P2.log";
char *sid_value = "none";
using namespace std;

void getSidForCurrentOwner() {
    const int length = 256;
    PSID sid = NULL;
    char *domain = NULL;
    DWORD sid_size = 0;
    DWORD domain_size = 0;
    SID_NAME_USE use = SidTypeUnknown;

    TCHAR name[257];
    DWORD bufferSize = sizeof(name);
    if (!GetUserNameA(name, &bufferSize)) {
        printf("Nu s a putut obtine numele");
    }
    LookupAccountNameA(NULL, name, NULL, &sid_size, NULL, &domain_size, &use);
    sid = (PSID) LocalAlloc(LMEM_FIXED, sid_size);
    domain = (TCHAR *) malloc(domain_size * sizeof(TCHAR));
    if (!LookupAccountNameA(NULL, name, sid, &sid_size, domain, &domain_size, &use)) {
        printf("Eroare la obtinerea sid");
    } else {
        //printf("S a obtinut sidul\n");
        ConvertSidToStringSidA(sid, &sid_value);

    }
}


void write_in_file(const char *path, char *text) {

    HANDLE handle;
    BOOL ErrorFlag;
    DWORD NumberOfLetters = (DWORD) strlen(text);
    DWORD NumberOfLettersWritten = 0;
    handle = CreateFileA(path, FILE_APPEND_DATA, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
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

void createRegistryKey() {
    HKEY key;


    if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\CSSO\\Tema5", 0, KEY_SET_VALUE, &key) != ERROR_SUCCESS) {
        printf("Eroare la cheie");
        write_in_file(log_path,"Nu se poate accesa reg_key\n");
        printf("%u", GetLastError);
    } else {


        DWORD dwDisposition;
        HKEY phkResult;
        char value[50];
        strcpy(value, sid_value);
        strcpy(value, value + 37);
        for (int i = 0; i < strlen(value); i++) {
            if (value[i] == '-') {
                strcpy(value + i, value + i + 1);
                i--;
            }
        }
        char registry_value[7];
        for (int i = 0; i < 6; i++) {
            registry_value[i] = value[i];
        }
        registry_value[6] = '\0';
        printf("value: %s", value);
        if (!RegCreateKeyExA(key, "P2", 0, "", 0, KEY_READ | KEY_WRITE, 0, &phkResult, &dwDisposition)) {

            if (RegSetValueEx(phkResult, TEXT("SID"), 0, REG_SZ, (const BYTE *) registry_value,
                              (lstrlen(registry_value)) + 1) != ERROR_SUCCESS) {
                printf("Nu s a reusit setarea valorii in cheie1");
                write_in_file(log_path, "Nu s-a reusit setarea valorilor\n");
                RegCloseKey(phkResult);
            } else {
                write_in_file(log_path, "A reusit setarea valorilor\n");
                RegCloseKey(phkResult);
            }

        }
        RegCloseKey(key);
    }


}

void createProcess(const char *processPath) {
    STARTUPINFOA startupInfo;
    PROCESS_INFORMATION processInformation;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    ZeroMemory(&processInformation, sizeof(processInformation));
    startupInfo.cb = sizeof(startupInfo);
    if (!CreateProcessA(processPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInformation)) {
        printf("Eroare la crearea procesului p1");
        write_in_file(log_path, "Nu s-a reusit pornirea lui P3\n");
    } else {
        WaitForSingleObject(processInformation.hProcess, INFINITE);
        write_in_file(log_path, "A reusit pornirea lui P3\n");
        CloseHandle(processInformation.hProcess);
        CloseHandle(processInformation.hThread);
    }
}


void write_sid_in_file() {
    getSidForCurrentOwner();
    write_in_file(path, sid_value);
    if (strstr(sid_value, "none")) {
        write_in_file(log_path, "Nu s-a reusit scrierea fisierului\n");
    } else {
        write_in_file(log_path, "A reusit scrierea fisierului\n");
    }
}

int main() {
    write_sid_in_file();
    //getSidForCurrentOwner();
    createRegistryKey();
    createProcess("D:\\facultate\\Anul 3\\Semestrul 1\\CSSO\\tema 5\\P3.exe");
    return 0;
}