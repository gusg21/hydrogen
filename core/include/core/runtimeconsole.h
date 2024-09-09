//
// Created by Angus Goucher on 9/9/2024.
//

#pragma once

#include <string>
#include <deque>
#include <unordered_map>

#include "SDL.h"

#define RUNTIMECONSOLE_LOG_LENGTH 50
#define RUNTIMECONSOLE_ENTRY_SIZE 1024

namespace h_core {
typedef void (*RuntimeConsoleCommandCallback)(const std::string& arguments, void* data);
typedef std::unordered_map<std::string, std::pair<RuntimeConsoleCommandCallback, void*>> CommandMap;

class RuntimeConsole {
  public:
    void init();
    void printToConsole(const std::string& message);
    void doGUI();
    void newCommand(const std::string& command, RuntimeConsoleCommandCallback callback, void* data);
    void runCommand(const std::string& command);

  private:
    static void logCallback(void *userdata, int category, SDL_LogPriority priority, const char *message);

    std::deque<std::string> m_log {RUNTIMECONSOLE_LOG_LENGTH, ""};
    char m_textEntry[RUNTIMECONSOLE_ENTRY_SIZE] = { 0 };
    bool m_logJustUpdated = false;
    CommandMap m_commands {};
};
}