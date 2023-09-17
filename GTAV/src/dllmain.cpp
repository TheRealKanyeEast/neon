#include "pch.h"
#include "core/core.h"
#include "security/security.h"
int DllMain(HINSTANCE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        //exit(0);
        DisableThreadLibraryCalls(module);
        //security::HideModule(module, false);
        /*if (HANDLE hThread = CreateThread(nullptr, 0, base::core::load, module, 0, nullptr)) {
            CloseHandle(hThread);
        }*/
        CreateThread(nullptr, 0, base::core::load, module, 0, nullptr);
    }

    return true;
}

