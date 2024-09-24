//
// Created by angus.goucher on 9/23/2024.
//

#include "editor/platform/invoke.h"

#include <Windows.h>

#include "core/log.h"

uint32_t h_editor::platform::invokeBlocking(const std::string& command, char* args) {
    HYLOG_DEBUG("INVOKE: %s %s", command.c_str(), args);

    STARTUPINFO info = { sizeof(info) };
    PROCESS_INFORMATION processInfo;
    if (CreateProcess(command.c_str(), args, nullptr, nullptr, TRUE, 0, nullptr, nullptr, &info, &processInfo)) {
        WaitForSingleObject(processInfo.hProcess, INFINITE);
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
    }
    return GetLastError();
}