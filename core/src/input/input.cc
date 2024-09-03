//
// Created by Angus Goucher on 9/2/2024.
//

#include "imgui.h"

#include "core/input/input.h"
#include "core/input/dualkeyinputactionsource.h"
#include "core/input/keyinputactionsource.h"
#include "core/window.h"

void h_core::input::Input::init(h_core::project::Project* project, h_core::Window* window) {
    m_window = window;

    m_currentKeys = SDL_GetKeyboardState(nullptr);
    m_previousKeys = new uint8_t[SDL_NUM_SCANCODES];

    for (h_core::project::ProjectActionEntry actionEntry : project->actions) {
        h_core::input::InputAction* inputAction =
            new h_core::input::InputAction();
        inputAction->name = actionEntry.name;
        for (h_core::project::ProjectActionSourceEntry sourceEntry :
             actionEntry.sources) {
            h_core::input::InputActionSource* source;
            switch (sourceEntry.type) {
                case h_core::input::InputActionSourceType::KEY:
                    source = new h_core::input::KeyInputActionSource(
                        sourceEntry.value.scanCode);
                    break;
                case h_core::input::InputActionSourceType::DUAL_KEY:
                    source = new h_core::input::DualKeyInputActionSource(
                        sourceEntry.value.dualScanCodes[0],
                        sourceEntry.value.dualScanCodes[1]);
                    break;
            }
            inputAction->sources.push_back(source);
        }
        m_actions.push_back(inputAction);
        m_actionNameMap[std::hash<std::string>()(inputAction->name)] =
            m_actions.size() - 1;
    }
}

void h_core::input::Input::updateInternals() {
    ::memcpy(m_previousKeys, m_currentKeys, SDL_NUM_SCANCODES);
    for (InputAction* action : m_actions) {
        action->lastFrameValue = action->getValue(this);
    }

//    SDL_SetWindowGrab(m_window->getSDLWindow(), mouseCaptured ? SDL_TRUE : SDL_FALSE);
    SDL_SetRelativeMouseMode(mouseCaptured ? SDL_TRUE : SDL_FALSE);

//    if (mouseCaptured) {
//        int width, height;
//        SDL_GetWindowSize(m_window->getSDLWindow(), &width, &height);
//        SDL_WarpMouseInWindow(m_window->getSDLWindow(), width / 2, height / 2);
//    }
}

float h_core::input::Input::getAnalogValue(
    h_core::input::InputActionIndex actionIndex) {
    return m_actions[actionIndex]->getValue(this);
}

bool h_core::input::Input::getDigitalValue(
    h_core::input::InputActionIndex actionIndex, float deadzone) {
    return ::abs(m_actions[actionIndex]->getValue(this)) > deadzone;
}

bool h_core::input::Input::getDigitalPressed(h_core::input::InputActionIndex actionIndex, float deadzone) {
    bool nowDown = ::abs(m_actions[actionIndex]->getValue(this)) > deadzone;
    bool wasDown = ::abs(m_actions[actionIndex]->lastFrameValue) > deadzone;
    return nowDown && !wasDown;
}

bool h_core::input::Input::getDigitalReleased(h_core::input::InputActionIndex actionIndex, float deadzone) {
    bool nowDown = ::abs(m_actions[actionIndex]->getValue(this)) > deadzone;
    bool wasDown = ::abs(m_actions[actionIndex]->lastFrameValue) > deadzone;
    return !nowDown && wasDown;
}

bool h_core::input::Input::isKeyDown(SDL_Scancode key) {
    return m_currentKeys[key];
}

const std::vector<h_core::input::InputAction*>*
    h_core::input::Input::getActions() {
    return &m_actions;
}

h_core::input::InputAction* h_core::input::Input::getAction(
    h_core::input::InputActionIndex actionIndex) {
    return m_actions[actionIndex];
}

h_core::input::InputActionIndex h_core::input::Input::newAction(
    const std::string& name) {
    h_core::input::InputAction* action = new h_core::input::InputAction();

    action->name = name;

    m_actions.push_back(action);
    h_core::input::InputActionIndex index = m_actions.size() - 1;
    m_actionNameMap[std::hash<std::string>()(name)] = index;

    return index;
}

h_core::input::InputActionIndex h_core::input::Input::getActionIndex(
    const std::string& actionName) {
    return m_actionNameMap[std::hash<std::string>()(actionName)];
}

float h_core::input::Input::getMouseX() {
    return m_mousePos.x;
}

float h_core::input::Input::getMouseY() {
    return m_mousePos.y;
}

float h_core::input::Input::getMouseDeltaX() {
    return m_mouseDelta.x;
}

float h_core::input::Input::getMouseDeltaY() {
    return m_mouseDelta.y;
}

void h_core::input::Input::setMousePos(uint32_t x, uint32_t y) {
    m_mousePos.x = x;
    m_mousePos.y = y;
}

void h_core::input::Input::setMouseDelta(int32_t x, int32_t y) {
    m_mouseDelta.x = x;
    m_mouseDelta.y = y;
}

void h_core::input::Input::doGUI() {
    if (ImGui::Begin("Input Debugger")) {
        ImGui::Text("SDL_NUM_SCANCODES: %d", SDL_NUM_SCANCODES);
        ImGui::Text("Mouse Captured: %s", mouseCaptured ? "YES" : "NO");
        ImGui::Text("Mouse Position: %.3f, %.3f", m_mousePos.x, m_mousePos.y);
        ImGui::Text("Mouse Delta: %.3f, %.3f", m_mouseDelta.x, m_mouseDelta.y);

        if (ImGui::CollapsingHeader("Actions")) {
            ImGui::Indent();

            uint32_t actionIndex = 0;
            for (const h_core::input::InputAction* action : *getActions()) {
                ImGui::PushID(actionIndex);
                if (ImGui::CollapsingHeader(action->name.c_str())) {
                    ImGui::BeginTable(
                        "Project Info", 2,
                        ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp);
                    ImGui::TableSetupColumn("Source Type", 0, 20.f);
                    ImGui::TableSetupColumn("Source Info", 0, 100.f);
                    ImGui::TableHeadersRow();

                    for (const h_core::input::InputActionSource* source : action->sources) {
                        ImGui::TableNextColumn();
                        ImGui::Text("%s", source->getName());
                        ImGui::TableNextColumn();
                        switch (source->type) {
                            case input::InputActionSourceType::KEY: {
                                const h_core::input::KeyInputActionSource* keySource =
                                    dynamic_cast<const h_core::input::KeyInputActionSource*>(source);
                                ImGui::Text(
                                    "%s (%d)", SDL_GetKeyName(SDL_SCANCODE_TO_KEYCODE(keySource->scanCode)),
                                    keySource->scanCode);
                                break;
                            }
                            case input::InputActionSourceType::DUAL_KEY: {
                                const h_core::input::DualKeyInputActionSource* keySource =
                                    dynamic_cast<const h_core::input::DualKeyInputActionSource*>(source);
                                ImGui::Text(
                                    "POS %s (%d), NEG %s (%d)",
                                    SDL_GetKeyName(SDL_SCANCODE_TO_KEYCODE(keySource->positiveScanCode)),
                                    keySource->positiveScanCode,
                                    SDL_GetKeyName(SDL_SCANCODE_TO_KEYCODE(keySource->negativeScanCode)),
                                    keySource->negativeScanCode);
                                break;
                            }
                            default:
                                ImGui::TextColored(
                                    ImVec4 { 0.8f, 0.93f, 0.12f, 1.0f }, "action source id %u unimplemented",
                                    (uint32_t)source->type);
                                break;
                        }
                    }

                    ImGui::EndTable();
                }
                ImGui::PopID();

                actionIndex++;
            }

            ImGui::Unindent();
        }
    }
    ImGui::End();
}
