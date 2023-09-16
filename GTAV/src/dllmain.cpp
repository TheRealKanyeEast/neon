#include "pch.h"
#include "core/core.h"
#include "security/security.h"
int DllMain(HMODULE module, DWORD reason, LPVOID reserve) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        //exit(0);
        DisableThreadLibraryCalls(module);
        //security::HideModule(module, false);
        if (HANDLE hThread = CreateThread(nullptr, 0, base::core::load, nullptr, 0, nullptr)) {
            CloseHandle(hThread);
        }
        break;
    
    case DLL_PROCESS_DETACH:

        break;
    }
    return true;
}

