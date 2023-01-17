#include <iostream>
#include <windows.h>
#include <aclapi.h>

using namespace std;

PSID getSidForEveryone() {
    PSID everyoneSID = NULL;
    SID_IDENTIFIER_AUTHORITY SIDAuthWorld =
            SECURITY_WORLD_SID_AUTHORITY;

    if (!AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &everyoneSID)) {
        printf("Eroare la obtinerea sid everyone");
    } else {
        printf("S a obtinut sid la everyone");
        return everyoneSID;
    }
}

void createProcess(const char *processPath) {
    STARTUPINFOA startupInfo;
    PROCESS_INFORMATION processInformation;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    ZeroMemory(&processInformation, sizeof(processInformation));
    startupInfo.cb = sizeof(startupInfo);
    if (!CreateProcessA(processPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInformation)) {
        printf("Eroare la crearea procesului");
    } else {
        WaitForSingleObject(processInformation.hProcess, INFINITE);
        CloseHandle(processInformation.hProcess);
        CloseHandle(processInformation.hThread);
    }
}

PSID getSidForCurrentOwner() {
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

        return sid;
    }
}

void delete_reg_key() {
    HKEY key;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\CSSO\\Tema5\\P3", 0, KEY_SET_VALUE, &key) != ERROR_SUCCESS) {
        printf("Eroare la cheie");
        printf("%u", GetLastError);
    } else {
        if (!RegDeleteKeyExA(key, "SIDEVERYONE", KEY_WOW64_32KEY, 0)) {
            printf("S-a reusit stergerea SIDEVERYONE");
        } else if (!RegDeleteKeyExA(key, "SIDEVERYONE", KEY_WOW64_64KEY, 0)) {
            printf("S-a reusit stergerea SIDEVERYONE");
        } else {
            printf("Nu se poate sterge SIDEVERYONE");
        }
    }
}

void change_rights_for_reg_key() {
    PSECURITY_DESCRIPTOR sd;
    EXPLICIT_ACCESS ea[3];
    PACL oldACL, newDACL;
    SE_OBJECT_TYPE objType = SE_REGISTRY_KEY;
    PSID everyone = getSidForEveryone();
    PSID owner = getSidForCurrentOwner();
    if (SetEntriesInAcl(0, NULL, NULL, &oldACL) != ERROR_SUCCESS) {
        printf("Eroare la initializarea oldACL");
    }

    if (SetEntriesInAcl(0, NULL, NULL, &newDACL) != ERROR_SUCCESS) {
        printf("Eroare la initializarea oldACL");
    }
    //The GetNamedSecurityInfo function retrieves a copy of the security descriptor for an object specified by name.
    if (GetNamedSecurityInfoA("CURRENT_USER\\Software\\CSSO\\Tema5", objType, DACL_SECURITY_INFORMATION, NULL, NULL,
                              &oldACL, NULL, &sd) != ERROR_SUCCESS) {
        printf("Eroare la obtinerea sd");
    } else {
        ZeroMemory(&ea, 2*sizeof(EXPLICIT_ACCESS));


        ea[0].grfAccessPermissions = KEY_WRITE;
        ea[0].grfAccessMode = DENY_ACCESS;
        ea[0].grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
        ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
        ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
        ea[0].Trustee.ptstrName = (LPTSTR) everyone;

        ea[1].grfAccessPermissions = KEY_READ;
        ea[1].grfAccessMode = SET_ACCESS;
        ea[1].grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
        ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
        ea[1].Trustee.TrusteeType = TRUSTEE_IS_USER;
        ea[1].Trustee.ptstrName = (LPTSTR) owner;
        DWORD dwRes = SetEntriesInAclA(2, ea, NULL, &newDACL);
        if (dwRes != ERROR_SUCCESS) {
            printf("Eroare la modificarea ACL pentru cheia de registru");
        } else {
            SetNamedSecurityInfoA("CURRENT_USER\\Software\\CSSO\\Tema5", objType, DACL_SECURITY_INFORMATION, NULL, NULL,
                                  newDACL, NULL);
            printf("A reusit");
        }

    }

}


int main() {

    createProcess("D:\\facultate\\Anul 3\\Semestrul 1\\CSSO\\tema 5\\P1.exe");
    createProcess("D:\\facultate\\Anul 3\\Semestrul 1\\CSSO\\tema 5\\P2.exe");
    delete_reg_key();
    change_rights_for_reg_key();
    createProcess("D:\\facultate\\Anul 3\\Semestrul 1\\CSSO\\tema 5\\P2.exe");
}
