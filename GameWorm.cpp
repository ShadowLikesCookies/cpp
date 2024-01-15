
#include <iostream>

#include <Windows.h>
#include <TLHelp32.h>

HANDLE hProc = NULL;
DWORD pID;

bool attachProc(char* procName)
{
    PROCESSENTRY32 procEntry32;

    procEntry32.dwSize = sizeof(PROCESSENTRY32);

    auto hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcSnap == INVALID_HANDLE_VALUE)
    {
        std::cout << "FAILED HANDLE VALUE" << std::endl;
        return false;
    }
    while (Process32Next(hProcSnap, &procEntry32))
    {
        std::cout << procEntry32.szExeFile << std::endl;

        if (!strcmp(procName, procEntry32.szExeFile))
        {
            std::cout << "Found process" << procEntry32.szExeFile << "with process ID" << procEntry32.th32ProcessID << std::endl;
            hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry32.th32ProcessID);
            pID = procEntry32.th32ProcessID;

            if (hProc == NULL)
            {
                std::cout << "FAILED GETTING HANDLE TO PROCESS" << std::endl;
                CloseHandle(hProcSnap);
                return true;
            }
        }
    }
    std::cout << "PROCESS NOT FOUND" << std::endl;

    CloseHandle(hProcSnap);
    return false;
}

template <class dataType>
void wpm(dataType valToWrite, DWORD addressToWrite)
{
    WriteProcessMemory(hProc, (PVOID)addressToWrite, &valToWrite, sizeof(dataType), 0);
}

template <class dataType>
dataType rpm(DWORD addressToRead)
{
    dataType rpmBuffer;

    ReadProcessMemory(hProc, (PVOID)addressToRead, &rpmBuffer, sizeof(dataType), 0);
    return rpmBuffer;
}

int main()
{
    DWORD memoryAddress = 0x2090003DE50;

    attachProc((char*)"GameWorm.exe");
    while (true) {
        wpm<int>(100, memoryAddress);
    }

}


