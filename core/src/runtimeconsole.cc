//
// Created by Angus Goucher on 9/9/2024.
//

#include "core/runtimeconsole.h"

#include "imgui.h"


uint32_t callback_print(const std::string& arguments, void* data) {
    h_core::RuntimeConsole* self = (h_core::RuntimeConsole*)data;
    self->printToConsole(arguments);
    return 0;
}

uint32_t callback_clear(const std::string& arguments, void* data) {
    h_core::RuntimeConsole* self = (h_core::RuntimeConsole*)data;
    self->clearConsole();
    return 0;
}

void h_core::RuntimeConsole::init() {
    SDL_LogSetOutputFunction(h_core::RuntimeConsole::logCallback, this);

    newCommandWithHelp("print", callback_print, this, "[unformatted args] prints args to console");
    newCommandWithHelp("clear", callback_clear, this, "[no args] clears the console");
}

void h_core::RuntimeConsole::printToConsole(const std::string& message) {
    m_log.push_back(message);
    if (m_log.size() > RUNTIMECONSOLE_LOG_LENGTH) m_log.pop_front();

    m_logJustUpdated = true;
}

int h_core::RuntimeConsole::consoleInputCallback(ImGuiInputTextCallbackData* data) {
    h_core::RuntimeConsole* console = (h_core::RuntimeConsole*)data->UserData;
    switch (data->EventFlag) {
        case ImGuiInputTextFlags_CallbackCompletion: {
            const char* wordEnd = data->Buf + data->CursorPos;
            const char* wordStart = data->Buf;

            std::string word { wordStart, wordEnd };

            for (h_core::CommandMap::iterator iterator = console->m_commands.begin();
                 iterator != console->m_commands.end(); iterator++) {
                if (iterator->first.rfind(word, 0) == 0) {
                    data->DeleteChars(0, word.size());
                    data->InsertChars(data->CursorPos, iterator->first.c_str());
                    data->InsertChars(data->CursorPos, " ");
                }
            }

            break;
        }
        case ImGuiInputTextFlags_CallbackHistory:
        {
            // Example of HISTORY
            const int prev_history_pos = console->m_historyPos;
            if (data->EventKey == ImGuiKey_UpArrow)
            {
                if (console->m_historyPos == -1)
                    console->m_historyPos = console->m_history.size() - 1;
                else if (console->m_historyPos > 0)
                    console->m_historyPos--;
            }
            else if (data->EventKey == ImGuiKey_DownArrow)
            {
                if (console->m_historyPos != -1)
                    if (++console->m_historyPos >= console->m_history.size())
                        console->m_historyPos = -1;
            }

            // A better implementation would preserve the data on the current input line along with cursor position.
            if (prev_history_pos != console->m_historyPos)
            {
                std::string history_str = (console->m_historyPos >= 0) ? console->m_history[console->m_historyPos] : "";
                data->DeleteChars(0, data->BufTextLen);
                data->InsertChars(0, history_str.c_str());
            }
            break;
        }
    }
    return 0;
}

void h_core::RuntimeConsole::doGUI() {
    if (ImGui::Begin("Runtime Console", nullptr, ImGuiWindowFlags_NoNav)) {
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
        if (ImGui::InputText(
                "##Input", m_textEntry, RUNTIMECONSOLE_ENTRY_SIZE,
                ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion |
                    ImGuiInputTextFlags_CallbackHistory,
                consoleInputCallback, this)) {
            runCommand(m_textEntry);
            m_historyPos = -1;
            m_history.emplace_back(m_textEntry);
            memset(m_textEntry, 0, sizeof(char) * RUNTIMECONSOLE_ENTRY_SIZE);
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
    newCommandWithHelp(command, callback, data, "");
}

void h_core::RuntimeConsole::newCommandWithHelp(
    const std::string& command, h_core::RuntimeConsoleCommandCallback callback, void* data, const std::string& help) {
    m_commands.emplace(command, RuntimeCommand { data, help, callback });
}

uint32_t h_core::RuntimeConsole::runCommand(const std::string& command) {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "> %s", command.c_str());

    size_t spacePos = command.find_first_of(' ');
    std::string firstWord;
    if (spacePos != std::string::npos) { firstWord = command.substr(0, spacePos); }
    else { firstWord = command; }

    if (firstWord == "help" || firstWord == "?") {
        showHelp();
        return 0;
    }

    for (CommandMap::iterator iterator = m_commands.begin(); iterator != m_commands.end(); iterator++) {
        if (iterator->first == firstWord) {
            // callback(args, data)
            uint32_t result = 0;
            if (spacePos != std::string::npos) {
                result = iterator->second.callback(command.substr(spacePos + 1), iterator->second.data);
            }
            else { result = iterator->second.callback("", iterator->second.data); }
            if (result != 0) {
                SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "BAD COMMAND (error %d)", result);
                SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Help: %s", iterator->second.help.c_str());
            }
            return result;
        }
    }
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Unknown command %s", firstWord.c_str());
}

void h_core::RuntimeConsole::showHelp() {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "=== COMMAND LIST: ===");
    for (CommandMap::iterator iterator = m_commands.begin(); iterator != m_commands.end(); iterator++) {
        if (iterator->second.help.empty()) {
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "\t%s", iterator->first.c_str());
        }
        else {
            SDL_LogInfo(
                SDL_LOG_CATEGORY_APPLICATION, "\t%s - %s", iterator->first.c_str(), iterator->second.help.c_str());
        }
    }
}

void h_core::RuntimeConsole::clearConsole() {
    m_log.clear();
    m_log = std::deque<std::string> { RUNTIMECONSOLE_LOG_LENGTH, "" };
}
