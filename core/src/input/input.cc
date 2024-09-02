//
// Created by Angus Goucher on 9/2/2024.
//

#include "core/input/input.h"
#include "core/input/keyinputactionsource.h"

void h_core::input::Input::init(h_core::project::Project* project) {
    m_currentKeys = SDL_GetKeyboardState(nullptr);
    m_previousKeys = new uint8_t[SDL_NUM_SCANCODES];

    for (h_core::project::ProjectActionEntry actionEntry : project->actions) {
        h_core::input::InputAction* inputAction = new h_core::input::InputAction();
        inputAction->name = actionEntry.name;
        for (h_core::project::ActionSourceEntry sourceEntry : actionEntry.sources) {
            h_core::input::InputActionSource* source;
            switch (sourceEntry.type) {
                case h_core::input::InputActionSourceType::KEY:
                    source = new h_core::input::KeyInputActionSource();
                    break;
            }
            source->type = sourceEntry.type;
            inputAction->sources.push_back(source);
        }
        m_actions.push_back(inputAction);
        m_actionNameMap[std::hash<std::string>()(inputAction->name)] = m_actions.size() - 1;
    }
}

void h_core::input::Input::swapInputBuffers() {
    memcpy(m_previousKeys, m_currentKeys, SDL_NUM_SCANCODES);

//    for (uint32_t scanCode = 0; scanCode < SDL_NUM_SCANCODES; scanCode++) {
//        m_previousKeys[scanCode] = m_currentKeys[scanCode];
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

h_core::input::InputActionIndex h_core::input::Input::newAction(const std::string& name) {
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
