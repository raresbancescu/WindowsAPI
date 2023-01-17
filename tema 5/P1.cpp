#include <iostream>
#include <windows.h>
#include <sddl.h>
#include <aclapi.h>


using namespace std;
char standardPath[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week5";
char firstPath[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week5\\RESTRICTED_WRITE";
char secondPath[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week5\\LOGS";

PSID getSidForEveryone() {
    PSID everyoneSID = NULL;
    SID_IDENTIFIER_AUTHORITY SIDAuthWorld =
            SECURITY_WORLD_SID_AUTHORITY;

    if (!AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &everyoneSID)) {
        printf("Eroare la obtinerea sid everyone");
    } else {
        printf("S a obtinut sid la everyone");
        char *sidvalue;
        ConvertSidToStringSidA(everyoneSID, &sidvalue);
        printf("%s", sidvalue);
        return everyoneSID;
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
        printf("S a obtinut sidul");
        char *sidvalue;
        ConvertSidToStringSidA(sid, &sidvalue);
        printf("%s", sidvalue);
        return sid;
    }
}

void createDirectory(char path[], SECURITY_ATTRIBUTES sa) {


    if (CreateDirectory(path, &sa) == 0) {
        printf("Nu s a putut crea folderul %d", GetLastError());
    }

}

void create_folder_security() {
    SECURITY_DESCRIPTOR sd;
    EXPLICIT_ACCESS ea[4];
    PACL pACL = NULL;
    DWORD dwRes;
    SECURITY_ATTRIBUTES sa;
    PSID owner = getSidForCurrentOwner();
    PSID everyone = getSidForEveryone();
    memset(&sd, 0, sizeof(sd));
    ZeroMemory(&ea, 3 * sizeof(EXPLICIT_ACCESS));
    ea[0].grfAccessPermissions = FILE_GENERIC_READ;
    ea[0].grfAccessMode = SET_ACCESS;
    ea[0].grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
    ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
    ea[0].Trustee.ptstrName = (LPTSTR) everyone;

    ea[1].grfAccessPermissions = FILE_GENERIC_WRITE;
    ea[1].grfAccessMode = SET_ACCESS;
    ea[1].grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
    ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea[1].Trustee.TrusteeType = TRUSTEE_IS_USER;
    ea[1].Trustee.ptstrName = (LPTSTR) owner;
    ea[2] = ea[1];
    ea[2].grfAccessPermissions = FILE_GENERIC_READ;
    dwRes = SetEntriesInAclA(3, ea, NULL, &pACL);
    if (dwRes != ERROR_SUCCESS) {
        printf("Eroare la crearea ACL");
    }
    if (InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION) == false) {
        printf("Eroare la initializarea security descriptor");
    } else {
        printf("S a reusit initializarea security descriptor");
    }

    if (!SetSecurityDescriptorDacl(&sd, TRUE, pACL, FALSE)) {
        printf("Eroare la setarea DACL");
    }
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = &sd;
    sa.bInheritHandle = false;
    printf("A reusit");
    createDirectory(standardPath, sa);
    createDirectory(firstPath, sa);
    createDirectory(secondPath, sa);
}

void create_registry_key() {
    HKEY key;
    LPCTSTR directory = TEXT("SOFTWARE");
    SECURITY_DESCRIPTOR sd;
    EXPLICIT_ACCESS ea[3];
    PACL pACL = NULL;
    DWORD dwRes;
    SECURITY_ATTRIBUTES sa;
    PSID owner = getSidForCurrentOwner();
    PSID everyone = getSidForEveryone();
    memset(&sd, 0, sizeof(sd));
    ZeroMemory(&ea, 2 * sizeof(EXPLICIT_ACCESS));
    ea[0].grfAccessPermissions = KEY_READ;
    ea[0].grfAccessMode = SET_ACCESS;
    ea[0].grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
    ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
    ea[0].Trustee.ptstrName = (LPTSTR) everyone;

    ea[1].grfAccessPermissions = KEY_WRITE;
    ea[1].grfAccessMode = SET_ACCESS;
    ea[1].grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
    ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea[1].Trustee.TrusteeType = TRUSTEE_IS_USER;
    ea[1].Trustee.ptstrName = (LPTSTR) owner;
    dwRes = SetEntriesInAclA(2, ea, NULL, &pACL);
    if (dwRes != ERROR_SUCCESS) {
        printf("Eroare la crearea ACL pentru cheia de registru");
    }
    if (InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION) == false) {
        printf("Eroare la initializarea security descriptor pentru cheia de registru");
    } else {
        printf("S a reusit initializarea security descriptor pentru cheia de registru");
    }

    if (!SetSecurityDescriptorDacl(&sd, TRUE, pACL, FALSE)) {
        printf("Eroare la setarea DACL pentru cheia de registru");
    }
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = &sd;
    sa.bInheritHandle = false;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, directory, 0, KEY_SET_VALUE, &key) != ERROR_SUCCESS) {
        printf("Eroare la cheie");
        printf("%u", GetLastError);
    } else {
        DWORD dwDisposition;
        HKEY phkResult;
        HKEY phk2Result;

        if (!RegCreateKeyExA(key, "CSSO", 0, "", 0, KEY_READ|KEY_WRITE, &sa, &phkResult, &dwDisposition)) {
            if (!RegCreateKeyExA(phkResult, "Tema5", 0, "", 0, KEY_READ|KEY_WRITE, &sa, &phk2Result, &dwDisposition)) {
                printf("S a reusit crearea cheii");
            }

        }
        RegCloseKey(phkResult);
    }
    RegCloseKey(key);
}


int main() {
    create_folder_security();
    create_registry_key();
    return 0;
}
