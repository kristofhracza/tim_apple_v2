#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "sdk/schemaSystem/Schema.hpp"
#include "sdk/interfaces/Interfaces.hpp"
#include "sdk/Attributes.hpp"

#include "hooks/Hooks.hpp"

#include "gui/Gui.hpp"
#include "utilities/Logger.hpp"



void MainThread(HMODULE hModule) {
    Logger::Write("MainThread","Starting setup");
    InterfaceFunctions::SetInterfaces();
    Logger::Write("MainThread", "Interfaces setup");
    SchemaSystem::LoadAll();
    Logger::Write("MainThread", "Schema system setup");
    Hooks::Setup();
    Logger::Write("MainThread", "Hooks setup");

    while (!GetAsyncKeyState(VK_END))
        Sleep(100);

    Hooks::Destroy();
    SetWindowLongPtrW(GUI::hWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(GUI::pOldWndProc));
    GUI::Destroy();

    FreeLibraryAndExitThread(hModule, 0);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved){

    if (dwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        HANDLE thread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(MainThread), hModule, 0, nullptr);
        if (thread) CloseHandle(thread);
    }

    return TRUE;
}