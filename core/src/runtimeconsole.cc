//
// Created by Angus Goucher on 9/9/2024.
//

#include "core/runtimeconsole.h"

#include "imgui.h"


void callback_print(const std::string& arguments, void* data) {
    h_core::RuntimeConsole* self = (h_core::RuntimeConsole*)data;
    self->printToConsole(arguments);
}

void h_core::RuntimeConsole::init() {
    SDL_LogSetOutputFunction(h_core::RuntimeConsole::logCallback, this);

    newCommand("print", callback_print, this);
}

void h_core::RuntimeConsole::printToConsole(const std::string& message) {
    m_log.push_back(message);
    if (m_log.size() > RUNTIMECONSOLE_LOG_LENGTH) m_log.pop_front();

    m_logJustUpdated = true;
}

void h_core::RuntimeConsole::doGUI() {
    if (ImGui::Begin("Runtime Console")) {
        ImGui::BeginChild("Log", ImVec2 { 0, -30 });
        {
            for (const std::string& logItem : m_log) {
                ImGui::TextWrapped("%s", logItem.c_str());
            }
            ImGui::Spacing();

            if (m_logJustUpdated) {
                ImGui::SetScrollHereY(1.0f);
                m_logJustUpdated = false;
            }
        }
        ImGui::EndChild();
        ImGui::Separator();
        ImGui::SetNextItemWidth(-2);
        if (ImGui::InputText("##Input", m_textEntry, RUNTIMECONSOLE_ENTRY_SIZE, ImGuiInputTextFlags_EnterReturnsTrue)) {
            runCommand(m_textEntry);
        }
    }
    ImGui::End();
}

void h_core::RuntimeConsole::logCallback(void* userdata, int category, SDL_LogPriority priority, const char* message) {
    h_core::RuntimeConsole* self = (h_core::RuntimeConsole*)userdata;

    std::string fullMessage {};
    switch (priority) {
        case SDL_LOG_PRIORITY_VERBOSE:
            fullMessage += "VERBOSE: ";
            break;
        case SDL_LOG_PRIORITY_DEBUG:
            fullMessage += "DEBUG: ";
            break;
        case SDL_LOG_PRIORITY_INFO:
            fullMessage += "INFO: ";
            break;
        case SDL_LOG_PRIORITY_WARN:
            fullMessage += "WARN: ";
            break;
        case SDL_LOG_PRIORITY_ERROR:
            fullMessage += "ERROR: ";
            break;
        case SDL_LOG_PRIORITY_CRITICAL:
            fullMessage += "CRITICAL: ";
            break;
        case SDL_NUM_LOG_PRIORITIES:
            break;
    }

    fullMessage += message;

    self->printToConsole(fullMessage);
    printf("%s\n", fullMessage.c_str());
}

void h_core::RuntimeConsole::newCommand(
    const std::string& command, h_core::RuntimeConsoleCommandCallback callback, void* data) {
    m_commands.emplace(command, std::pair<RuntimeConsoleCommandCallback, void*> { callback, data });
}

void h_core::RuntimeConsole::runCommand(const std::string& command) {
    std::string firstWord = command.substr(0, command.find_first_of(' '));
    for (CommandMap::iterator iterator = m_commands.begin(); iterator != m_commands.end(); iterator++) {
        if (iterator->first == firstWord) {
            // callback(args, data)
            iterator->second.first(command.substr(command.find(' ') + 1), iterator->second.second);
            return;
        }
    }
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Unknown command %s", firstWord.c_str());
}

// void h_core::RuntimeConsole::printToConsole(const std::string& message) {}
