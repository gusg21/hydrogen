//
// Created by Angus Goucher on 9/9/2024.
//

#pragma once

#include <deque>
#include <string>
#include <unordered_map>

#include "SDL.h"
#include "imgui.h"
#include "yaml-cpp/yaml.h"

#include "core/math/color.h"

#define RUNTIMECONSOLE_LOG_LENGTH 50
#define RUNTIMECONSOLE_ENTRY_SIZE 1024

#define RUNTIMECONSOLE_PARSE_ARGS(args, outputVar)              \
    YAML::Node outputVar;                                       \
    try {                                                       \
        outputVar = YAML::Load(args);                           \
    } catch (YAML::ParserException&) {                          \
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Bad YAML"); \
        return 1;                                               \
    }

namespace h_core {
typedef uint32_t (*RuntimeConsoleCommandCallback)(const std::string& arguments, void* data);

class RuntimeCommand {
  public:
    RuntimeCommand(void* data, const std::string& help, RuntimeConsoleCommandCallback callback)
        : data(data), help(help), callback(callback) {}

    void* data = nullptr;
    std::string help {};
    RuntimeConsoleCommandCallback callback = nullptr;
};

#undef ERROR
enum class LogLevel { VERBOSE, DEBUG, INFO, WARN, ERROR, CRITICAL };

struct RuntimeConsoleMessage {
  public:
    RuntimeConsoleMessage() = default;
    RuntimeConsoleMessage(const std::string& message, h_core::math::Color color, LogLevel level)
        : message(message), color(color), level(level) {}

    std::string message {};
    h_core::math::Color color { 1.0f, 1.0f, 1.0f, 1.0f };
    LogLevel level = LogLevel::INFO;
};


typedef std::unordered_map<std::string, RuntimeCommand> CommandMap;

class RuntimeConsole {
  public:
    void init();
    void printToConsole(const std::string& message, LogLevel level);
    void printToConsoleColored(const std::string& message, LogLevel level, h_core::math::Color color);
    void doGUI();
    void newCommand(const std::string& command, RuntimeConsoleCommandCallback callback, void* data);
    void newCommandWithHelp(
        const std::string& command, RuntimeConsoleCommandCallback callback, void* data, const std::string& help);
    uint32_t runCommand(const std::string& command);
    void clearConsole();

  private:
    void showHelp();
    static int consoleInputCallback(ImGuiInputTextCallbackData* data);
    static void logCallback(void* userdata, int category, SDL_LogPriority priority, const char* message);

    std::deque<RuntimeConsoleMessage> m_log { RUNTIMECONSOLE_LOG_LENGTH };
    LogLevel m_minimumLevel = LogLevel::VERBOSE;
    char m_textEntry[RUNTIMECONSOLE_ENTRY_SIZE] = { 0 };
    std::vector<std::string> m_history {};
    int32_t m_historyPos = 0;
    bool m_logJustUpdated = false;
    CommandMap m_commands {};
};
}  // namespace h_core