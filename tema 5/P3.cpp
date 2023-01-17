#include <iostream>
#include <aclapi.h>
#include <sddl.h>
#include <string.h>

using namespace std;

char *sid_everyone = "none";
char *sid_admin = "none";
char path[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week5\\RESTRICTED_WRITE\\P3.txt";
char log_path[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week5\\LOGS\\P3.log";

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

void getSidForEveryone() {
    PSID everyoneSID = NULL;
    SID_IDENTIFIER_AUTHORITY SIDAuthWorld =
            SECURITY_WORLD_SID_AUTHORITY;

    if (!AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &everyoneSID)) {
        printf("Eroare la obtinerea sid everyone");
    } else {
        //printf("S a obtinut sid la everyone");
        ConvertSidToStringSidA(everyoneSID, &sid_everyone);
    }
}

PSID getSidForAdministrators() {
    PSID administratorsSID = NULL;
    SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;

    if (!AllocateAndInitializeSid(&SIDAuthNT, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0,
                                  &administratorsSID)) {
    } else {
        //printf("S a obtinut sid la admin");
        ConvertSidToStringSidA(administratorsSID, &sid_admin);
    }
}

void createRegistryKeys() {
    HKEY key;
    LPCTSTR directory = TEXT("SOFTWARE\\CSSO\\Tema5");
    // creare cheie de registru p3 aici
    if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\CSSO\\Tema5", 0, KEY_SET_VALUE, &key) != ERROR_SUCCESS) {
        printf("Eroare la cheie");
        write_in_file(log_path,"Nu se poate accesa reg_key\n");
        printf("%u", GetLastError);
    } else {

        int ok = 1;

        DWORD dwDisposition;
        HKEY phkResult;
        char everyone_value[50];
        char admin_value[50];
        strcpy(everyone_value, sid_everyone);
        strcpy(admin_value, sid_admin);

        char everyone_registry_value[7];
        int index = 0;
        for (int i = 0; i < strlen(everyone_value) && index <= 5; i++) {
            if (everyone_value[i] <= '9' && everyone_value[i] >= '0') {
                everyone_registry_value[index++] = everyone_value[i];
            }
        }
        everyone_registry_value[index] = '\0';
        char admin_registry_value[7];
        index = 0;
        for (int i = 0; i < strlen(admin_value) && index <= 5; i++) {
            if (admin_value[i] <= '9' && admin_value[i] >= '0') {
                admin_registry_value[index++] = admin_value[i];
            }
        }
        admin_registry_value[index] = '\0';
        printf("values: %s : %s", everyone_registry_value, admin_registry_value);

        if (!RegCreateKeyExA(key, "P3", 0, "", 0, KEY_READ | KEY_WRITE, 0, &phkResult, &dwDisposition)) {
            if (RegSetValueEx(phkResult, TEXT("SIDEVERYONE"), 0, REG_SZ, (const BYTE *) everyone_registry_value,
                              (lstrlen(everyone_registry_value)) + 1) != ERROR_SUCCESS) {
                printf("Nu s a reusit setarea valorii everyone in cheie1");
                ok = 0;
            }


            if (RegSetValueEx(phkResult, TEXT("SIDADMIN"), 0, REG_SZ, (const BYTE *) admin_registry_value,
                              (lstrlen(admin_registry_value)) + 1) != ERROR_SUCCESS) {
                printf("Nu s a reusit setarea valorii admin in cheie1");
                ok = 0;
            }
            if (ok == 1) {
                write_in_file(log_path, "A reusit crearea subkeys\n");
            } else {
                write_in_file(log_path, "Nu s-a reusit crearea subkeys\n");
            }
        }
        RegCloseKey(phkResult);

    }
    RegCloseKey(key);

}

void get_sids() {
    getSidForEveryone();
    getSidForAdministrators();
    write_in_file(path, sid_admin);
    write_in_file(path,"\n");
    write_in_file(path, sid_everyone);
    if (strstr(sid_admin, "none") || strstr(sid_everyone, "none")) {
        write_in_file(log_path, "Nu s-a reusit scrierea fisierului\n");
    } else {
        write_in_file(log_path, "S-a reusit scrierea fisierului\n");
    }
}

int main() {
    printf("Testam din programul P3");
    get_sids();
    createRegistryKeys();
    return 0;
}
